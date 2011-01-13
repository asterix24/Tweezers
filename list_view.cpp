/**
 * \file
 * <!--
 * Tweezers is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Copyright 2010 Daniele Basile <asterix@develer.com>
 *
 * -->
 *
 * \brief List view manager.
 *
 * \author Daniele Basile <asterix@develer.com>
 */


#include "list_view.h"

#include <cfg/cfg_tweezers.h>

#include <QDebug>

#define FILE_COL       0
#define PREVIEW_COL    1

void ListView::clean()
{
    table->clear();
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setVisible(false);
    table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setVisible(false);
    table->setColumnCount(0);
    table->setRowCount(0);
}

void ListView::addFiles(QString path, QStringList files)
{
    glob_list.clear();
    items.clear();

    for (QStringList::iterator f = files.begin();
            f != files.end(); f++)
    {
        QFileInfo fi(*f);
        QString suff = fi.suffix().toLower();
        ItemNode *item = new ItemNode(path, *f, "-",suff);
        glob_list[suff] = 0;
        items.append(*item);
    }
}

QStringList ListView::getGlobs()
{
    return glob_list.keys();
}

void ListView::showFiles()
{
    int len = items.length();
    int index = 0;

    clean();
    table->hide();
    table->setColumnCount(2);
    table->setSortingEnabled(false);
    table->setRowCount(len);

    for (QList<ItemNode>::iterator it = items.begin();
        it != items.end(); it++)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem((*it).origin_name);
        QTableWidgetItem *item1 = new QTableWidgetItem((*it).new_name);

        table->setItem(index, FILE_COL, item0);
        table->setItem(index, PREVIEW_COL, item1);

        index++;
    }
    table->show();
    qApp->processEvents();
}

void ListView::setExpression(QString exp)
{
    QRegExp rx(TAG_PATTEN);

    int pos = 0;
    tag_list.clear();
    expression = exp;

    while (1)
    {
        pos = rx.indexIn(expression, pos);
        if (pos < 0)
            break;

        tag_list << rx.cap(1);
        pos += rx.matchedLength();
    }
}

void ListView::preview()
{
    for (QList<ItemNode>::iterator it = items.begin();
        it != items.end(); it++)
    {
        (*it).expression = expression;
        (*it).new_name = tag_converter->fill_tags((*it).path, (*it).origin_name, (*it).expression, tag_list);
    }
}

void ListView::rename()
{
//    table->item(count, FILE_COL)->setIcon(QIcon("./images/ok.png"));
//    table->item(count, FILE_COL)->setText(table->item(count, PREVIEW_COL)->text());
//    table->item(count, PREVIEW_COL)->setText("");

#if 0
    table->initIterator();
    while(table->hasNext())
    {
        if (rename_selected_only && table->currItemNotSelected())
        {
            table->next();
            continue;
        }

        QString origin = curr_path + QDir::separator () + table->getFile();
        QString renamed = curr_path + QDir::separator () + table->getFilePreview();

        QFile origin_filename(origin);

        // The are two file with same name, we skip it.
        if (backup.contains(renamed))
        {
            table->setRenamedWarning();
            table->next();
            count_warning++;
            continue;
        }

        if (origin_filename.rename(renamed))
        {
            backup[renamed] = origin;
            table->setRenamedOk();
            count_renamed++;
        }
        else /* Rename fail, set warning in the layout */
        {
            table->setRenamedError();
            count_error++;
        }

        statusBar()->showMessage(tr("Renamed: ") + QString::number(count_renamed) +
                                 tr(" Warning: ") +  QString::number(count_warning) +
                                 tr(" Error: ") + QString::number(count_error));
        table->next();
    }
#endif
}





bool ListView::currItemNotSelected(void)
{
    return !table->item(count, FILE_COL)->isSelected();
}

bool ListView::areItemsSelected(void)
{
    QList<QTableWidgetItem *> list;
    list = table->selectedItems();

    qDebug() << list.size();
    return list.size();
}


ListView::ListView(QTableWidget *t, TagConverter *tag)
{
    table = t;
    tag_converter = tag;
    clean();
};

ListView::~ListView()
{
}


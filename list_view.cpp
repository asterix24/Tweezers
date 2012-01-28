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
#include "itemnode.h"

#include <cfg/cfg_tweezers.h>

#include <QFileInfo>
#include <QFile>
#include <QDebug>

#define FILE_COL       0
#define PREVIEW_COL    1
#define ITEM_REF       1

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
    extension_list.clear();
    items.clear();

    for (QStringList::iterator f = files.begin();
    f != files.end(); f++)
    {
        QFileInfo fi(*f);
        QString suff = fi.suffix().toLower();
        extension_list[suff] = 0;
        items.append(ItemNode(path, *f, "-",suff));
    }
}

void ListView::showFiles()
{
    int len = items.length();
    item_counter = 0;

    clean();
    table->hide();
    table->setColumnCount(2);
    table->setSortingEnabled(false);
    table->setRowCount(len);

    for (QList<ItemNode>::iterator it = items.begin(); it != items.end(); it++)
    {
        QVariant item_ref;
        item_ref.setValue<ItemNode *>(&(*it));
        QTableWidgetItem *item0 = new QTableWidgetItem((*it).origin_name);
        QTableWidgetItem *item1 = new QTableWidgetItem((*it).new_name);

        item0->setData(ITEM_REF, item_ref);
        table->setItem(item_counter, FILE_COL, item0);
        table->setItem(item_counter, PREVIEW_COL, item1);

        item_counter++;
}
    table->show();
    qApp->processEvents();
}

ItemNode ListView::item(int row)
{
    QTableWidgetItem *item = table->item(row, FILE_COL);
    QVariant ref = item->data(ITEM_REF);
    return *ref.value<ItemNode *>();
}

QStringList ListView::extractTagList(QString expression)
{
    QStringList tag_list;
    int pos = 0;

    if (expression.isEmpty())
            return tag_list;

    QRegExp rx(TAG_PATTEN);

    while (1)
    {
        pos = rx.indexIn(expression, pos);
        if (pos < 0)
                break;

        tag_list << rx.cap(1);
        pos += rx.matchedLength();
    }

    return tag_list;
}

void ListView::preview(QString expression)
{
    QStringList tag_list = extractTagList(expression);
    for (int i = 0; i < items.size(); i++)
    {
        if (tag_list.isEmpty())
            items[i].new_name = expression;
        else
            tag->fill_tags(&items[i], expression, tag_list);
    }
}

void ListView::rename()
{
    item_renamed = 0;
    item_not_renamed = 0;

    for (QList<ItemNode>::iterator f = items.begin(); f != items.end(); f++)
    {
        (*f).renamed = false;
        QFile origin_filename((*f).full_origin_name);

        if (origin_filename.rename((*f).path + QDir::separator() + (*f).new_name))
        {
            (*f).renamed = true;
            (*f).prev_name = (*f).origin_name;
            (*f).origin_name = (*f).new_name;
            (*f).new_name = "";
            item_renamed++;
        }
        else
        {
            item_not_renamed++;
        }
    }
}

void ListView::undoRename()
{

    item_renamed = 0;
    item_not_renamed = 0;
    for (QList<ItemNode>::iterator f = items.begin(); f != items.end(); f++)
    {
        if ((*f).renamed)
        {
            QFile renamed_filename((*f).path + QDir::separator() + (*f).origin_name);
            QString uno = (*f).path + QDir::separator() + (*f).origin_name;

            if (renamed_filename.rename((*f).path + QDir::separator() + (*f).prev_name))
            {
                (*f).renamed = false;
                (*f).prev_name = (*f).origin_name;
                item_renamed++;
            }
            else
            {
                item_not_renamed++;
            }
        }
    }
}




ListView::ListView(QTableWidget *t, TagConverter *_tag)
{
    table = t;
    tag = _tag;
    clean();
};

ListView::~ListView()
{
}


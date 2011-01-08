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
        ItemNode item(path, *f, suff);
        glob_list[suff] = 0;
        items.append(item);
    }
}

QStringList ListView::getGlobs()
{
    return glob_list.keys();
}

void ListView::showFiles(QString glob)
{
    int len = items.length();

    clean();
    table->hide();
    table->setColumnCount(2);
    table->setSortingEnabled(false);
    table->setRowCount(len);

    for (int i = 0; i < len; i++)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem(items[i].origin);
        QTableWidgetItem *item1 = new QTableWidgetItem("");

        table->setItem(i, FILE_COL, item0);
        table->setItem(i, PREVIEW_COL, item1);
    }
    table->show();
    qApp->processEvents();
}

void ListView::fill(const QStringList col)
{
    clean();

    if (col.empty())
        return;

    table->hide();
    table->setColumnCount(2);
    table->setSortingEnabled(false);
    table->setRowCount(col.length());

    for (int i = 0; i < col.length(); i++)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem(col[i]);
        QTableWidgetItem *item1 = new QTableWidgetItem("");

        table->setItem(i, FILE_COL, item0);
        table->setItem(i, PREVIEW_COL, item1);
    }
    table->show();
    qApp->processEvents();
}

void ListView::initIterator()
{
    count = 0;
    table->hide();
}

bool ListView::hasNext()
{
    if (count >= table->rowCount())
    {
        table->show();
        return false;
    }
    return true;
}

void ListView::next()
{
    count++;
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

void ListView::setFilePreview(QString text)
{
    table->item(count, PREVIEW_COL)->setText(text);
}

void ListView::setFile(QString text)
{
    table->item(count, FILE_COL)->setText(text);
}

QString ListView::getFile(void)
{
    return table->item(count, FILE_COL)->text();
}

QString ListView::getFilePreview(void)
{
    return table->item(count, PREVIEW_COL)->text();
}

void ListView::setRenamedOk(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/ok.png"));
    table->item(count, FILE_COL)->setText(table->item(count, PREVIEW_COL)->text());
    table->item(count, PREVIEW_COL)->setText("");
}

void ListView::setRenamedWarning(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/warning.png"));
}


void ListView::setRenamedError(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/error.png"));
}

ListView::ListView(QTableWidget *t)
{
    table = t;
    clean();
};

ListView::~ListView()
{
}


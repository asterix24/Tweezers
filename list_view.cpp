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

void ListView::fill(QStringList col)
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
}

bool ListView::hasNext()
{
        return (count < table->rowCount());
}


void ListView::hide()
{
        table->hide();
}

void ListView::show()
{
        table->show();
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
    QString file = table->item(count, FILE_COL)->text();
    return file;
}

QString ListView::getFilePreview(void)
{
    QString file = table->item(count, PREVIEW_COL)->text();
    return file;
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


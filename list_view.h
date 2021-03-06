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

#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "itemnode.h"
#include "tag_converter.h"

#include <QtGui>

class ListView
{
private:
    TagConverter *tag;
    QTableWidget *table;

    QList<ItemNode> items;
    QHash<QString, int> extension_list;

    // Statistic
    int item_counter;
    int item_renamed;
    int item_not_renamed;

    QStringList extractTagList(QString expression);
public:
    void clean();

    void addFiles(QString path, QStringList files);
    QStringList extensionList() { return extension_list.keys(); }
    int lenght() { return item_counter; }
    int renanmed() { return item_renamed; }
    int notRenanmed() { return item_not_renamed; }

    void preview(QString expression);
    void showFiles();
    void rename();
    void undoRename();

    ItemNode item(int row);

    ListView(QTableWidget *t, TagConverter *_tag);
    ~ListView();
};

#endif // LIST_VIEW_H

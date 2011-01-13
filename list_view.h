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

#include <QtGui>

#include "tag_converter.h"

struct ItemNode
{
    QString path;
    QString origin_name;
    QString new_name;
    QString expression;
    QString suffix;
    bool renamed;

    ItemNode(QString _path, QString _origin_name, QString _new_name, QString _suffix):
        path(_path), origin_name(_origin_name), new_name(_new_name), suffix(_suffix)
    {}
};


class ListView
{
private:
    QTableWidget *table;
    TagConverter *tag_converter;
    QList<ItemNode> items;
    QHash<QString, QString> backup;
    QHash<QString, int> glob_list;
    QList<QString> tag_list;
    QString expression;
    int index;

public:
    void clean();

    void addFiles(QString path, QStringList files);
    QStringList getGlobs();
    int getLoadedItems() { return index; }

    void showFiles();
    void rename();
    void setExpression(QString exp);
    void preview();

    bool currItemNotSelected();
    bool areItemsSelected();

    ListView(QTableWidget *t,  TagConverter *tag);
    ~ListView();
};

#endif // LIST_VIEW_H

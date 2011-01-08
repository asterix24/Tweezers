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

struct ItemNode
{
    QString origin;
    QString expression;
    QString path;
    QString suffix;
    bool renamed;

    ItemNode(QString _path, QString _origin, QString _suffix)
    {
        path = _path;
        origin = _origin;
        suffix = _suffix;
    }
};


class ListView
{
private:
    QTableWidget *table;
    QList<ItemNode> items;
    QHash<QString, int> glob_list;
    int count;

public:
    void clean();

    void addFiles(QString path, QStringList files);
    QStringList getGlobs();

    void showFiles(QString glob);
    void fill(const QStringList col);

    void initIterator();
    bool hasNext();
    void next();

    bool currItemNotSelected();
    bool areItemsSelected();
    void setFilePreview(QString text);
    void setFile(QString text);

    QString getFile();
    QString getFilePreview();

    void setRenamedOk();
    void setRenamedWarning();
    void setRenamedError();

    ListView(QTableWidget *t);
    ~ListView();
};

#endif // LIST_VIEW_H

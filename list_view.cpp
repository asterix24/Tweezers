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

#include <QFileInfo>
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
		ItemNode *item = new ItemNode(path, *f, "-",suff);
		extension_list[suff] = 0;
		items.append(*item);
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

	ItemNode *p;
	for (QList<ItemNode>::iterator it = items.begin();
	it != items.end(); it++)
	{
		p = &(*it);
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
	QRegExp rx(TAG_PATTEN);

	int pos = 0;
	QStringList tag_list;

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
		tag->fill_tags(&items[i], expression, tag_list);
	}
}

void ListView::rename()
{
	for (QList<ItemNode>::iterator f = items.begin(); f != items.end(); f++)
	{
		(*f).renamed = false;

		QFile origin_filename((*f).path + QDir::separator() + (*f).origin_name);
		if (origin_filename.rename((*f).path + QDir::separator() + (*f).new_name))
			(*f).renamed = true;
	}

	//    table->item(count, FILE_COL)->setIcon(QIcon("./images/ok.png"));
	//    table->item(count, FILE_COL)->setText(table->item(count, PREVIEW_COL)->text());
	//    table->item(count, PREVIEW_COL)->setText("");
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


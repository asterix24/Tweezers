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
	glob_list.clear();
	items.clear();

	for (QStringList::iterator f = files.begin();
	f != files.end(); f++)
	{
		QFileInfo fi(*f);
		QString suff = fi.suffix().toLower();
		ItemNode node;
		node.path = path;
		node.origin_name = *f;
		node.new_name = "-";
		node.suffix = suff;
		//ItemNode *item = new ItemNode(path, *f, "-",suff);
		glob_list[suff] = 0;
		items.append(node);
	}
}

QStringList ListView::getGlobs()
{
	return glob_list.keys();
}

void ListView::showFiles()
{
	int len = items.length();
	index = 0;

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
		table->setItem(index, FILE_COL, item0);
		table->setItem(index, PREVIEW_COL, item1);

		index++;
	}
	table->show();
	qApp->processEvents();

	qDebug() << p->origin_name << p->new_name;
}

QTableWidgetItem *ListView::getPreview(int row)
{
	return table->item(row, PREVIEW_COL);
}

QTableWidgetItem *ListView::getFile(int row)
{
	return table->item(row, FILE_COL);
}

ItemNode ListView::getItem(int row)
{
	QTableWidgetItem *item = table->item(row, FILE_COL);
	QVariant ref = item->data(ITEM_REF);
	return *ref.value<ItemNode *>();
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


ListView::ListView(QTableWidget *t, TagConverter *tag)
{
	table = t;
	tag_converter = tag;
	clean();
};

ListView::~ListView()
{
}


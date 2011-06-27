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
 * \brief Tag convert module.
 *
 * \author Daniele Basile <asterix@develer.com>
 */

#include "tag_converter.h"
#include "fileinfo.h"
#include <cfg/cfg_tweezers.h>

#include <QString>
#include <QChar>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QDate>

#include <iostream>
#include <iomanip>
#include <cassert>

static QString getExifdate(ItemNode *node, FileInfo *info, Preference *p)
{
	QString data_str = "";

	if (data_str.isEmpty())
		return "";

	QStringList data_list = data_str.split(" ");
	QString str = "";

	if (!data_list.isEmpty())
	{
		data_list = data_list[0].split(":");

		if (data_list.length() >= 3)
		{
			QDate date(data_list[0].toInt(), data_list[1].toInt(), data_list[2].toInt());
			str = date.toString(p->getDateFmt());
		}
	}

	return str;
}

static QString getExiftime(ItemNode *node, FileInfo *info, Preference *p)
{
	QString time_str = "";

	if (time_str.isEmpty())
		return "";

	QString str = "";
	QStringList time_list = time_str.split(" ");

	if (!time_list.isEmpty())
	{
		time_list = time_list[1].split(":");

		if (time_list.length() >= 3)
		{
			QTime time(time_list[0].toInt(), time_list[1].toInt(), time_list[2].toInt());
			str = time.toString(p->getTimeFmt());
		}
	}

	return str;
}

static QString getExt(ItemNode *node, FileInfo *info, Preference *p)
{
	return node->suffix;
}

static QString getExtLow(ItemNode *node, FileInfo *info, Preference *p)
{
	return node->suffix.toLower();
}

static QString getExtUp(ItemNode *node, FileInfo *info, Preference *p)
{
	return node->suffix.toUpper();
}


static QString getName(ItemNode *node, FileInfo *info, Preference *p)
{
	qDebug() << node->origin_name;
	return node->origin_name;
}

static QString getNameLow(ItemNode *node, FileInfo *info, Preference *p)
{
	return node->origin_name.toLower();
}

static QString getNameUp(ItemNode *node, FileInfo *info, Preference *p)
{
	return node->origin_name.toUpper();
}


QString TagConverter::fill_tags(ItemNode *node, QList<QString> tag_list)
{
	FileInfo *info = new FileInfo(*node);
	for (int i = 0; i < tag_list.length(); ++i)
	{
		if (callback_table.contains(tag_list[i].toLower()))
		{
			// Search the right function to convert tag
			tag_callback foo = callback_table[tag_list[i].toLower()];

			// Call the right function to convert tag
			QString  tag_value = foo(node, info, preference);
			node->expression = node->expression.replace(tag_list[i], tag_value);
		}
	}
	return node->expression;
}

QList<QString> TagConverter::getTagDesc()
{
	return descr_table;
}

TagConverter::TagConverter(Preference *pref)
{
	preference = pref;
	descr_table << tr("Elenco dei tag..");

	callback_table["<exiftime>"] = getExiftime;
	descr_table << tr("<EXIFTIME>: Ritorna l'ora di scatto della foto");

	callback_table["<exifdate>"] = getExifdate;
	descr_table << tr("<EXIFDATE>: Ritorna la data di scatto della foto");

	callback_table["<ext>"] = getExt;
	descr_table << tr("<EXT>: Ritorna l'estensione del file");

	callback_table["<ext_low>"] = getExtLow;
	descr_table << tr("<EXT_LOW>: Ritorna l'estensione del file lowercase");

	callback_table["<ext_up>"] = getExtUp;
	descr_table << tr("<EXT_UP>: Ritorna l'estensione del file uppercase");

	callback_table["<name>"] = getName;
	descr_table << tr("<NAME>: Ritorna il nome del file senza estensione");

	callback_table["<name_low>"] = getNameLow;
	descr_table << tr("<NAME_LOW>: Ritorna il nome del file senza estensione lowercase");

	callback_table["<name_up>"] = getNameUp;
	descr_table << tr("<NAME_UP>: Ritorna il nome del file senza estensione uppercase");
}

TagConverter::~TagConverter()
{
}

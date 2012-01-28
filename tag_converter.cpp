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
#include <QObject>
#include <QFileInfo>

#include <iostream>
#include <iomanip>
#include <cassert>
#include <map>


static QString getExifdate(ItemNode *node, FileInfo *info, Preference *p)
{
    (void)node;

    QString data_str = info->dateTimeOriginal();

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
    (void)node;

    QString time_str = info->dateTimeOriginal();

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
    (void)node;
    (void)info;
    (void)p;
    return node->suffix;
}

static QString getExtLow(ItemNode *node, FileInfo *info, Preference *p)
{

    (void)node;
    (void)info;
    (void)p;
    return node->suffix.toLower();
}

static QString getExtUp(ItemNode *node, FileInfo *info, Preference *p)
{

    (void)node;
    (void)info;
    (void)p;
    return node->suffix.toUpper();
}


static QString getName(ItemNode *node, FileInfo *info, Preference *p)
{

    (void)node;
    (void)info;
    (void)p;
    QFileInfo name(node->origin_name);
    return name.baseName();
}

static QString getNameLow(ItemNode *node, FileInfo *info, Preference *p)
{

    (void)node;
    (void)info;
    (void)p;
    QFileInfo name(node->origin_name);
    return name.baseName().toLower();
}

static QString getNameUp(ItemNode *node, FileInfo *info, Preference *p)
{

    (void)node;
    (void)info;
    (void)p;
    QFileInfo name(node->origin_name);
    return name.baseName().toUpper();
}


TagNode tags[] =
{
    { "<exiftime>", getExiftime, QObject::tr("<EXIFTIME>: Ritorna l'ora di scatto della foto") },
    { "<exifdate>", getExifdate, QObject::tr("<EXIFDATE>: Ritorna la data di scatto della foto") },
    { "<ext>",      getExt,      QObject::tr("<EXT>: Ritorna l'estensione del file") },
    { "<ext_low>",  getExtLow,   QObject::tr("<EXT_LOW>: Ritorna l'estensione del file lowercase") },
    { "<ext_up>",   getExtUp,    QObject::tr("<EXT_UP>: Ritorna l'estensione del file uppercase") },
    { "<name>",     getName,     QObject::tr("<NAME>: Ritorna il nome del file senza estensione") },
    { "<name_low>", getNameLow,  QObject::tr("<NAME_LOW>: Ritorna il nome del file senza estensione minuscolo") },
    { "<name_up>",  getNameUp,   QObject::tr("<NAME_UP>: Ritorna il nome del file senza estensione maiuscolo") },
    { "",           NULL,        "" }
};

tag_callback TagConverter::callback(QString key)
{
    key = key.toLower();
    if (table.contains(key))
        return table[key].callback;

    return NULL;
}


void TagConverter::fill_tags(ItemNode *node, QString expression, QStringList tag_list)
{
    FileInfo *info = new FileInfo(*node);

#warning evalute the possibility to remove this field
    node->expression = expression;
    node->new_name = expression;

    for (QStringList::iterator tag_item = tag_list.begin(); tag_item != tag_list.end(); ++tag_item)
    {
        tag_callback call = callback((*tag_item));
        QString  tag_value = "";
        if (call)
        {
            tag_value = call(node, info, preference);
            node->new_name = node->new_name.replace((*tag_item), tag_value);
        }
    }
}


TagConverter::TagConverter(Preference *_preference)
{
    preference = _preference;
    for (int i = 0; tags[i].key != ""; i++)
    {
        table[tags[i].key] = tags[i];
        tag_descriptions.append(tags[i].description);
    }

}

TagConverter::~TagConverter()
{
}

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

#ifndef TAG_CONVERTER_H
#define TAG_CONVERTER_H

#include "preference.h"
#include "itemnode.h"
#include "fileinfo.h"

#include <QString>
#include <QHash>
#include <QObject>

#include <map>

typedef QString (*tag_callback)(ItemNode *node, FileInfo *info, Preference *p);

class TagConverter : public QObject
{
private:
	QHash<QString, tag_callback> callback_table;
	QList<QString> descr_table;

	Preference *preference;

public:
	QString fill_tags(ItemNode *node, QList<QString> tag_list);

	QList<QString> getTagDesc();

	TagConverter(Preference *pref);
	~TagConverter();
};

#endif // TAG_CONVERTER_H

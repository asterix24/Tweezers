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
 * Copyright 2011 Daniele Basile <asterix@develer.com>
 *
 * -->
 *
 * \brief File Info module.
 *
 * \author Daniele Basile <asterix@develer.com>
 */

#include "fileinfo.h"

#include <QString>
#include <QVector>
#include <QDebug>

#include <libexif/exif-data.h>

FileInfo::FileInfo(QString file_name)
{
	if (file_name.isEmpty())
		qDebug() << tr("Empty file name.");

	// Load an ExifData object from an EXIF file
	exif_data = exif_data_new_from_file(file_name.toStdString().c_str());

	if (!exif_data)
		qDebug() << tr("File not readable or no EXIF data in file ") << file_name;
}

QString FileInfo::showTag(ExifTag tag)
{
	// See if this tag exists
	ExifEntry *entry = exif_content_get_entry(exif_data->ifd[0], tag);
	if (entry)
	{
		char buf[1024];

		// Get the contents of the tag in human-readable form
		exif_entry_get_value(entry, buf, sizeof(buf));
		QString desc(exif_tag_get_name_in_ifd(tag, (ExifIfd)0));

		return desc + " " + QString(buf);
	}
	return "";
}

FileInfo::~FileInfo()
{
	// Free the EXIF data
	exif_data_unref(exif_data);
}


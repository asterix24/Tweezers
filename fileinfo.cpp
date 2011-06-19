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
#include <QDebug>

#include <libexif/exif-data.h>

ExifInfo::ExifInfo(QString file_name)
{
	if (file_name.isEmpty())
		qDebug() << tr("Empty file name.");

	// Load an ExifData object from an EXIF file
	ed = exif_data_new_from_file(file_name.toStdString().c_str());
	if (!ed)
		qDebug() << tr("File not readable or no EXIF data in file ") << file_name;
}

ExifInfo::~ExifInfo()
{
	/* Free the EXIF data */
	exif_data_unref(ed);
}


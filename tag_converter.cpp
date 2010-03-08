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
#include <cfg/cfg_tweezers.h>

#include <QString>
#include <QChar>
#include <QDir>
#include <QHash>

#include <exiv2/image.hpp>
#include <exiv2/exif.hpp>

#include <iostream>
#include <iomanip>
#include <cassert>


QString getExif(QString path, QString item, QString exif_tag)
{
    try
    {
        QString name = path + QDir::separator () + item;

        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(name.toStdString());

        assert(image.get() != 0);
        image->readMetadata();

        Exiv2::ExifData &exifData = image->exifData();
        if (exifData.empty())
        {
            std::string error(name.toStdString());
            return "";
        }

        return QString(exifData[exif_tag.toStdString()].toString().c_str());

    }
    catch (Exiv2::Error)
    {
        return "";
    }
}

QString getExifdate(QString path, QString item)
{
    QString data_str = getExif(path, item, "Exif.Image.DateTime");

    if (data_str.isEmpty())
        return "";

    QStringList data_list = data_str.split(" ");

    return data_list[0];
}

QString getExiftime(QString path, QString item)
{
    QString time_str = getExif(path, item, "Exif.Image.DateTime");
    QStringList time_list = time_str.split(" ");

    if (time_str.isEmpty())
        return "";

    return time_list[1];
}

QString getExt(QString path, QString item)
{
    QString name = path + QDir::separator () + item;
    QFileInfo fi(name);

    return fi.suffix();
}

QString TagConverter::fill_tags(QString path, QString item, QString exp, QList<QString> tag_list)
{
    for (int i = 0; i < tag_list.length(); ++i)
    {
        if (callback_table.contains(tag_list[i]))
        {
            tag_callback f = callback_table[tag_list[i]];

            QString  tag_value = f(path, item);
            exp = exp.replace(tag_list[i], tag_value);
        }
    }
    return exp;
}

TagConverter::TagConverter()
{
    callback_table["<exiftime>"] = getExiftime;
    callback_table["<exifdate>"] = getExifdate;
    callback_table["<ext>"] = getExt;
}

TagConverter::~TagConverter()
{
}

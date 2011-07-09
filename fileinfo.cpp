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
#include <QStringList>

#include <libexif/exif-data.h>

static const ExifTag tag_list[] =
{
	EXIF_TAG_INTEROPERABILITY_INDEX,
	EXIF_TAG_INTEROPERABILITY_VERSION,
	EXIF_TAG_NEW_SUBFILE_TYPE,
	EXIF_TAG_IMAGE_WIDTH,
	EXIF_TAG_IMAGE_LENGTH,
	EXIF_TAG_BITS_PER_SAMPLE,
	EXIF_TAG_COMPRESSION,
	EXIF_TAG_PHOTOMETRIC_INTERPRETATION,
	EXIF_TAG_FILL_ORDER,
	EXIF_TAG_DOCUMENT_NAME,
	EXIF_TAG_IMAGE_DESCRIPTION,
	EXIF_TAG_MAKE,
	EXIF_TAG_MODEL,
	EXIF_TAG_STRIP_OFFSETS,
	EXIF_TAG_ORIENTATION,
	EXIF_TAG_SAMPLES_PER_PIXEL,
	EXIF_TAG_ROWS_PER_STRIP,
	EXIF_TAG_STRIP_BYTE_COUNTS,
	EXIF_TAG_X_RESOLUTION,
	EXIF_TAG_Y_RESOLUTION,
	EXIF_TAG_PLANAR_CONFIGURATION,
	EXIF_TAG_RESOLUTION_UNIT,
	EXIF_TAG_TRANSFER_FUNCTION,
	EXIF_TAG_SOFTWARE,
	EXIF_TAG_DATE_TIME,
	EXIF_TAG_ARTIST,
	EXIF_TAG_WHITE_POINT,
	EXIF_TAG_PRIMARY_CHROMATICITIES,
	EXIF_TAG_SUB_IFDS,
	EXIF_TAG_TRANSFER_RANGE,
	EXIF_TAG_JPEG_PROC,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT,
	EXIF_TAG_JPEG_INTERCHANGE_FORMAT_LENGTH,
	EXIF_TAG_YCBCR_COEFFICIENTS,
	EXIF_TAG_YCBCR_SUB_SAMPLING,
	EXIF_TAG_YCBCR_POSITIONING,
	EXIF_TAG_REFERENCE_BLACK_WHITE,
	EXIF_TAG_XML_PACKET,
	EXIF_TAG_RELATED_IMAGE_FILE_FORMAT,
	EXIF_TAG_RELATED_IMAGE_WIDTH,
	EXIF_TAG_RELATED_IMAGE_LENGTH,
	EXIF_TAG_CFA_REPEAT_PATTERN_DIM,
	EXIF_TAG_CFA_PATTERN,
	EXIF_TAG_BATTERY_LEVEL,
	EXIF_TAG_COPYRIGHT,
	EXIF_TAG_EXPOSURE_TIME,
	EXIF_TAG_FNUMBER,
	EXIF_TAG_IPTC_NAA,
	EXIF_TAG_IMAGE_RESOURCES,
	EXIF_TAG_EXIF_IFD_POINTER,
	EXIF_TAG_INTER_COLOR_PROFILE,
	EXIF_TAG_EXPOSURE_PROGRAM,
	EXIF_TAG_SPECTRAL_SENSITIVITY,
	EXIF_TAG_GPS_INFO_IFD_POINTER,
	EXIF_TAG_ISO_SPEED_RATINGS,
	EXIF_TAG_OECF,
	EXIF_TAG_TIME_ZONE_OFFSET,
	EXIF_TAG_EXIF_VERSION,
	EXIF_TAG_DATE_TIME_ORIGINAL,
	EXIF_TAG_DATE_TIME_DIGITIZED,
	EXIF_TAG_COMPONENTS_CONFIGURATION,
	EXIF_TAG_COMPRESSED_BITS_PER_PIXEL,
	EXIF_TAG_SHUTTER_SPEED_VALUE,
	EXIF_TAG_APERTURE_VALUE,
	EXIF_TAG_BRIGHTNESS_VALUE,
	EXIF_TAG_EXPOSURE_BIAS_VALUE,
	EXIF_TAG_MAX_APERTURE_VALUE,
	EXIF_TAG_SUBJECT_DISTANCE,
	EXIF_TAG_METERING_MODE,
	EXIF_TAG_LIGHT_SOURCE,
	EXIF_TAG_FLASH,
	EXIF_TAG_FOCAL_LENGTH,
	EXIF_TAG_SUBJECT_AREA,
	EXIF_TAG_TIFF_EP_STANDARD_ID,
	EXIF_TAG_MAKER_NOTE,
	EXIF_TAG_USER_COMMENT,
	EXIF_TAG_SUB_SEC_TIME,
	EXIF_TAG_SUB_SEC_TIME_ORIGINAL,
	EXIF_TAG_SUB_SEC_TIME_DIGITIZED,
	EXIF_TAG_XP_TITLE,
	EXIF_TAG_XP_COMMENT,
	EXIF_TAG_XP_AUTHOR,
	EXIF_TAG_XP_KEYWORDS,
	EXIF_TAG_XP_SUBJECT,
	EXIF_TAG_FLASH_PIX_VERSION,
	EXIF_TAG_COLOR_SPACE,
	EXIF_TAG_PIXEL_X_DIMENSION,
	EXIF_TAG_PIXEL_Y_DIMENSION,
	EXIF_TAG_RELATED_SOUND_FILE,
	EXIF_TAG_INTEROPERABILITY_IFD_POINTER,
	EXIF_TAG_FLASH_ENERGY,
	EXIF_TAG_SPATIAL_FREQUENCY_RESPONSE,
	EXIF_TAG_FOCAL_PLANE_X_RESOLUTION,
	EXIF_TAG_FOCAL_PLANE_Y_RESOLUTION,
	EXIF_TAG_FOCAL_PLANE_RESOLUTION_UNIT,
	EXIF_TAG_SUBJECT_LOCATION,
	EXIF_TAG_EXPOSURE_INDEX,
	EXIF_TAG_SENSING_METHOD,
	EXIF_TAG_FILE_SOURCE,
	EXIF_TAG_SCENE_TYPE,
	EXIF_TAG_NEW_CFA_PATTERN,
	EXIF_TAG_CUSTOM_RENDERED,
	EXIF_TAG_EXPOSURE_MODE,
	EXIF_TAG_WHITE_BALANCE,
	EXIF_TAG_DIGITAL_ZOOM_RATIO,
	EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM,
	EXIF_TAG_SCENE_CAPTURE_TYPE,
	EXIF_TAG_GAIN_CONTROL,
	EXIF_TAG_CONTRAST,
	EXIF_TAG_SATURATION,
	EXIF_TAG_SHARPNESS,
	EXIF_TAG_DEVICE_SETTING_DESCRIPTION,
	EXIF_TAG_SUBJECT_DISTANCE_RANGE,
	EXIF_TAG_IMAGE_UNIQUE_ID,
	EXIF_TAG_GAMMA,
	EXIF_TAG_PRINT_IMAGE_MATCHING,
	EXIF_TAG_PADDING,
};

struct ExifTable
{
	int id;
	ExifTag tag;
};

static struct ExifTable exif_table[] =
{
   { 0, EXIF_TAG_IMAGE_DESCRIPTION },
   { 0, EXIF_TAG_MAKE },
   { 0, EXIF_TAG_MODEL },
   { 0, EXIF_TAG_ORIENTATION },
   { 0, EXIF_TAG_X_RESOLUTION },
   { 0, EXIF_TAG_Y_RESOLUTION },
   { 0, EXIF_TAG_RESOLUTION_UNIT },
   { 0, EXIF_TAG_SOFTWARE },
   { 0, EXIF_TAG_DATE_TIME },
   { 0, EXIF_TAG_YCBCR_POSITIONING },
   { 1, EXIF_TAG_X_RESOLUTION },
   { 1, EXIF_TAG_Y_RESOLUTION },
   { 1, EXIF_TAG_RESOLUTION_UNIT },
   { 1, EXIF_TAG_COMPRESSION },
   { 2, EXIF_TAG_CUSTOM_RENDERED },
   { 2, EXIF_TAG_EXPOSURE_MODE },
   { 2, EXIF_TAG_WHITE_BALANCE },
   { 2, EXIF_TAG_DIGITAL_ZOOM_RATIO },
   { 2, EXIF_TAG_FOCAL_LENGTH_IN_35MM_FILM },
   { 2, EXIF_TAG_SCENE_CAPTURE_TYPE },
   { 2, EXIF_TAG_GAIN_CONTROL },
   { 2, EXIF_TAG_CONTRAST },
   { 2, EXIF_TAG_SATURATION },
   { 2, EXIF_TAG_SHARPNESS },
   { 2, EXIF_TAG_SUBJECT_DISTANCE_RANGE },
   { 2, EXIF_TAG_EXPOSURE_TIME },
   { 2, EXIF_TAG_FNUMBER },
   { 2, EXIF_TAG_EXPOSURE_PROGRAM },
   { 2, EXIF_TAG_ISO_SPEED_RATINGS },
   { 2, EXIF_TAG_EXIF_VERSION },
   { 2, EXIF_TAG_DATE_TIME_ORIGINAL },
   { 2, EXIF_TAG_DATE_TIME_DIGITIZED },
   { 2, EXIF_TAG_COMPONENTS_CONFIGURATION },
   { 2, EXIF_TAG_COMPRESSED_BITS_PER_PIXEL },
   { 2, EXIF_TAG_SHUTTER_SPEED_VALUE },
   { 2, EXIF_TAG_APERTURE_VALUE },
   { 2, EXIF_TAG_EXPOSURE_BIAS_VALUE },
   { 2, EXIF_TAG_MAX_APERTURE_VALUE },
   { 2, EXIF_TAG_METERING_MODE },
   { 2, EXIF_TAG_LIGHT_SOURCE },
   { 2, EXIF_TAG_FLASH },
   { 2, EXIF_TAG_FOCAL_LENGTH },
   { 2, EXIF_TAG_SUBJECT_AREA },
   { 2, EXIF_TAG_MAKER_NOTE },
   { 2, EXIF_TAG_USER_COMMENT },
   { 2, EXIF_TAG_FLASH_PIX_VERSION },
   { 2, EXIF_TAG_COLOR_SPACE },
   { 2, EXIF_TAG_PIXEL_X_DIMENSION },
   { 2, EXIF_TAG_PIXEL_Y_DIMENSION },
   { 2, EXIF_TAG_SENSING_METHOD },
   { 2, EXIF_TAG_FILE_SOURCE },
   { 2, EXIF_TAG_SCENE_TYPE },
   { 3, EXIF_TAG_INTEROPERABILITY_INDEX },
   { 3, EXIF_TAG_INTEROPERABILITY_VERSION },
   { 4, EXIF_TAG_INTEROPERABILITY_INDEX },
   { 4, EXIF_TAG_INTEROPERABILITY_VERSION },
};


FileInfo::FileInfo(ItemNode &node)
{
	error = false;
	if (node.full_origin_name.isEmpty())
	{
		error = true;
		qDebug() << tr("Empty file name.");
	}

	loadExif(node.full_origin_name);
}

FileInfo::FileInfo(QString file_name)
{
	error = false;
	if (file_name.isEmpty())
	{
		error = true;
		qDebug() << tr("Empty file name.");
	}

	if (!error)
		loadExif(file_name);
}

void FileInfo::loadExif(QString file_name)
{
	exif_data = exif_data_new_from_file(file_name.toStdString().c_str());
	if (!exif_data)
	{
		error = true;
		//qDebug() << tr("File not readable or no EXIF data in file ") << file_name;
	}
}

QString FileInfo::showTag(ExifIfd id, ExifTag tag)
{
	if (error)
		return "";

	ExifEntry *entry = exif_content_get_entry(exif_data->ifd[id], tag);
	if (entry)
	{
		char buf[1024];
		exif_entry_get_value(entry, buf, sizeof(buf));
		return QString(exif_tag_get_name_in_ifd(tag, id)) + ": " + QString(buf);
	}

	return "";
}

QString FileInfo::rawTag(ExifIfd id, ExifTag tag)
{
	if (error)
		return "";

	ExifEntry *entry = exif_content_get_entry(exif_data->ifd[id], tag);
	if (entry)
	{
		char buf[1024];
		return QString(exif_entry_get_value(entry, buf, sizeof(buf)));
	}

	return "";
}

void FileInfo::showAll()
{

	for (int j = 0; j < EXIF_IFD_COUNT; j++)
	{
		for (size_t i = 0; i < sizeof(tag_list); i++)
		{
			QString s(showTag((ExifIfd)j, tag_list[i]));
			if (!s.isEmpty())
				qDebug() << s;
		}
	}
}

QStringList FileInfo::allExifTag()
{

	QStringList l;
	std::cout << "[\n";
	for (int j = 0; j < EXIF_IFD_COUNT; j++)
	{
		for (size_t i = 0; i < sizeof(tag_list); i++)
		{
			QString s(showTag((ExifIfd)j, tag_list[i]));
			if (!s.isEmpty())
			{
				l << s;
				std::cout << "\"" << j << " " << i << "\",\n";

			}
		}
	}
	std::cout << "]\n";
	return l;
}

FileInfo::~FileInfo()
{
	exif_data_unref(exif_data);
}


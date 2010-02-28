#include "tag_converter.h"

#include <QString>
#include <QChar>
#include <QDir>

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

QString TagConverter::fill_tags(QString path, QString item, QString exp)
{
    // TODO: move into config module.
    QRegExp rx("(<\\w+>)");

    int pos = 0;
    while (1)
    {
        pos = rx.indexIn(exp, pos);

        // No more tags we exit.
        if (pos < 0)
            break;

         // We find a valid tag we replace it with its value.
        if (callback_table.contains(rx.cap(1)))
        {
            tag_callback f = callback_table[rx.cap(1)];

            QString value = f(path, item);
            pos += value.length();
            exp = exp.replace(rx.cap(1), value);
        }
        else
            // No valid tag, skip it.
            pos += rx.matchedLength();
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

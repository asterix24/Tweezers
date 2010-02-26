#ifndef TAG_CONVERTER_H
#define TAG_CONVERTER_H

#include <QString>
#include <QHash>

typedef QString (*tag_callback)(QString path, QString item);
class TagConverter
{
public:
    QString fill_tags(QString path, QString item, QString exp);
    TagConverter();
    ~TagConverter();
private:
    QHash<QString, tag_callback> callback_table;
};


#endif // TAG_CONVERTER_H

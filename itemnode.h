#ifndef ITEMNODE_H
#define ITEMNODE_H

#include <QDir>
#include <QMetaType>

#include <iostream>

struct ItemNode
{
	QString path;
	QString origin_name;
	QString new_name;
	QString prev_name;
	QString expression;
	QString suffix;
	QString full_origin_name;
	bool renamed;

	ItemNode(QString _path, QString _origin_name, QString _new_name, QString _suffix):
			path(_path), origin_name(_origin_name), new_name(_new_name), suffix(_suffix)
	{
		full_origin_name = path + QDir::separator() + origin_name;
		prev_name = "";
	}
};

Q_DECLARE_METATYPE(ItemNode *);

inline std::ostream &operator<< (std::ostream &os, const ItemNode node)
{
	os << "Path " << node.path.toStdString() << "\n"
			<< "Origin name " << node.origin_name.toStdString() << "\n"
			<< "New name " << node.new_name.toStdString() << "\n"
			<< "Suffix " << node.suffix.toStdString() << "\n"
			<< "Expression " << node.expression.toStdString() << "\n";

	return os;
}
#endif // ITEMNODE_H

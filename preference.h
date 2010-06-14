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
 * \brief Tweezers preference window.
 *
 * \author Daniele Basile <asterix@develer.com>
 */
#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QtGui>
#include <QtGui/QWidget>
#include <QHash>

namespace Ui {
    class Preference;
}

class Preference : public QWidget {
    Q_OBJECT

public:    
    QString getPath() { return curr_path; }
    QString getLastExp() { return last_expr; }

    void setPath(QString path) { curr_path = path; }
    void setLastExp(QString expr) { last_expr = expr; }

    QPoint getPos() { return pos; }
    QSize getSize() { return size; }

    void setPos(QPoint p) { pos = p; }
    void setSize(QSize s) { size = s; }

    Preference(QWidget *parent = 0);
    ~Preference();

protected:
    void changeEvent(QEvent *e);

private slots:
    void ok(void);
    void cancel(void);
    void preview(QTableWidgetItem *item);
    void upDatePreview(QString str);

private:
    // Path settings
    QString curr_path;
    QString last_expr;

    // Windows settings
    QPoint pos;
    QSize size;

    //Default format
    QHash<QString, QString> category;

    QString curr_date_fmt;
    QString curr_time_fmt;

    void fillCategory();

    void readSettings();
    void writeSettings();

    void createActions();

    Ui::Preference *m_ui;
};

#endif // PREFERENCE_H

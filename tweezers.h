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
 * \brief Tweezers GUI.
 *
 * \author Daniele Basile <asterix@develer.com>
 */

#ifndef TWEEZERS_H
#define TWEEZERS_H

#include "tag_converter.h"
#include "preference.h"

#define FILE_COL       0
#define PREVIEW_COL    1

#include <QMainWindow>
#include <QFile>

namespace Ui {
    class Tweezers;
}

class ListView
{
private:
    QTableWidget *table;
    int count;

public:
    void clean();
    void fill(QStringList col);
    void fill(QStringList col_a, QStringList col_b);

    void initIterator();
    bool hasNext();
    void next();
    void setFilePreview(QString text);
    QString getFile(void);

    ListView(QTableWidget *t);
    ~ListView();
};



class Tweezers : public QMainWindow {
    Q_OBJECT
public:
    Tweezers(QWidget *parent = 0);
    ~Tweezers();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private slots:
    void preview();

    void openDir();
    void loadFiles(void);
    void selectDirectory();

    void selExpCombo(int index);

    void renameAll();
    void renameSelection();
    void undoRename();

    void preferences();
    void about();

    void exprTextMod();

private:
    ListView *table;
    QString curr_path;
    QStringList glob_exp;

    QHash<QString, QString> backup;

    // Application Actions
    QAction *preferenceAct;
    QAction *aboutAct;
    QAction *openAct;
    QAction *exitAct;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QTimer *timer;

    bool expr_changed;


    void renameList(QList<QTableWidgetItem *>items);

    // Init the application menu and actions
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

    // External class pointers
    Preference *preference_window;
    TagConverter *tag;
    Ui::Tweezers *ui;
};

#endif // TWEEZERS_H

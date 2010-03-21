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

#include "tweezers.h"
#include "ui_tweezers.h"
#include "tag_converter.h"

#include <cfg/cfg_tweezers.h>

#include <QtGui>

Tweezers::Tweezers(QWidget *parent) :
    QMainWindow(parent),
    tag(),
    ui(new Ui::Tweezers)
{
    curr_path =  QDir::home().absolutePath();

    ui->setupUi(this);
    ui->selectDir->setText(curr_path);
    ui->globSelect->setText("*.*");
    ui->expField->setText("");
    ui->expList->addItems(tag.getTagDesc());

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setUnifiedTitleAndToolBarOnMac(true);

}

Tweezers::~Tweezers()
{
    delete ui;
}

void Tweezers::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Tweezers::openDir()
{
    curr_path = QFileDialog::getExistingDirectory(this, QString(tr("Open Directory")), curr_path, QFileDialog::ShowDirsOnly);

    if (! curr_path.isEmpty())
        ui->selectDir->setText(curr_path);
}

void Tweezers::selectDirectory()
{
    curr_path = ui->selectDir->text();

    if (! curr_path.isEmpty())
        loadFiles();
}

void Tweezers::loadFiles(void)
{
    QDir dir(curr_path);

    glob_exp.clear();
    if (ui->globSelect->isModified())
        glob_exp << ui->globSelect->text();

    QStringList file_list;
    file_list = dir.entryList(glob_exp, QDir::Files);

    if(!file_list.length())
    {
        cleanTable();
        return;
    }

    ui->fileList->clear();
    ui->fileList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->fileList->horizontalHeader()->setVisible(true);
    ui->fileList->verticalHeader()->setVisible(false);

    ui->fileList->setColumnCount(2);
    ui->fileList->setRowCount(file_list.length());

    for (int i = 0; i < file_list.length(); i++)
    {   
        QTableWidgetItem *item0 = new QTableWidgetItem(file_list[i]);
        QTableWidgetItem *item1 = new QTableWidgetItem("");
        ui->fileList->setItem(i, FILE_COL, item0);
        ui->fileList->setItem(i, PREVIEW_COL, item1);
    }
}

void Tweezers::preview()
{
    //
    QRegExp rx(TAG_PATTEN);
    QList<QString> tag_list;
    QString exp = ui->expField->text();
    int pos = 0;

    while (1)
    {
        pos = rx.indexIn(exp, pos);

        // No more tags we exit.
        if (pos < 0)
            break;

        /*
         *  we find the tag, make it lower case and save into list
         */
        tag_list << rx.cap(1);

        // search forward into expr string
        pos += rx.matchedLength();
    }

    // Update the table view
    for (int i = 0; i < ui->fileList->rowCount(); i++)
    {
        QString value = tag.fill_tags(curr_path, ui->fileList->item(i, FILE_COL)->text(), ui->expField->text(),
                                      tag_list);
        ui->fileList->item(i, PREVIEW_COL)->setText(value);
    }
}

void Tweezers::renameAll()
{
    for (int i = 0; i < ui->fileList->rowCount(); i++)
    {
        QString origin = curr_path + QDir::separator () + ui->fileList->item(i, FILE_COL)->text();
        QString renamed = curr_path + QDir::separator () + ui->fileList->item(i, PREVIEW_COL)->text();

        QFile origin_filename(origin);

        if (backup.contains(renamed))
            continue;

        if (origin_filename.rename(renamed))
        {
            backup[renamed] = origin;
            ui->fileList->item(i, FILE_COL)->setText(ui->fileList->item(i, PREVIEW_COL)->text());
            ui->fileList->item(i, PREVIEW_COL)->setText("");
        }
    }
}

void Tweezers::renameSelection()
{
}

/*
 * Undo last files rename
 */
void Tweezers::undoRename()
{
    QHashIterator<QString, QString> i(backup);
    while (i.hasNext())
    {
        i.next();
        QFile renamed_filename(i.key());
        renamed_filename.rename(i.value());
    }
    backup.clear();
    loadFiles();
    preview();
}

/*
 * Fill expr edit line with tags select from combo tag list.
 */
void Tweezers::selExpCombo(int index)
{

    if (!index)
        return;

    QStringList list = ui->expList->itemText(index).split(":");

    if (!list.isEmpty())
        ui->expField->insert(list.at(0));
}

/*
 * Clean all table widget
 */
void Tweezers::cleanTable()
{
    ui->fileList->clear();
    ui->fileList->horizontalHeader()->setVisible(false);
    ui->fileList->setColumnCount(0);
    ui->fileList->setRowCount(0);
}

void Tweezers::preferences()
{

}

 void Tweezers::about()
 {
    QMessageBox::about(this, tr("About Application"),
             tr("The <b>Application</b> example demonstrates how to "
                "write modern GUI applications using Qt, with a menu bar, "
                "toolbars, and a status bar."));
 }

void Tweezers::createActions()
{

    // Preference window action
    preferenceAct = new QAction(tr("&Preference.."), this);
    preferenceAct->setShortcuts(QKeySequence::Preferences);
    preferenceAct->setStatusTip(tr("Tweezers preference"));
    connect(preferenceAct, SIGNAL(triggered()), this, SLOT(preferences()));

    // About window action
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    // Manage all directory widget signals
    connect(ui->selDirButton, SIGNAL(clicked()), this, SLOT(openDir()));
    connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(loadFiles()));
    connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));

    // Manage all expression field signals
    connect(ui->expField, SIGNAL(textChanged(const QString)), this, SLOT(preview()));

    // Manage the rename action signals
    connect(ui->doRename, SIGNAL(clicked()), this, SLOT(renameAll()));
    connect(ui->undoRename, SIGNAL(clicked()), this, SLOT(undoRename()));

    // Manage tag list signals
    connect(ui->expList, SIGNAL(activated(int)), this, SLOT(selExpCombo(int)));

    // Manage list file to rename
    // TODO: Complicare la struttura dei data contenuti nella tabella.
    //connect(ui->fileList, SIGNAL(itemActivated (QTableWidgetItem)), this, SLOT());
}

void Tweezers::createMenus()
{
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(preferenceAct);
     fileMenu->addSeparator();

     editMenu = menuBar()->addMenu(tr("&Edit"));

     menuBar()->addSeparator();

     helpMenu = menuBar()->addMenu(tr("&Help"));
     helpMenu->addAction(aboutAct);

 }

 void Tweezers::createToolBars()
 {
     fileToolBar = addToolBar(tr("File"));

     editToolBar = addToolBar(tr("Edit"));
 }

 void Tweezers::createStatusBar()
 {
     statusBar()->showMessage(tr("Ready"));
 }

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
#include "preference.h"

#include "ui_preference.h"
#include "ui_tweezers.h"

#include "tag_converter.h"

#include <cfg/cfg_tweezers.h>

#include <QtGui>


void ListView::clean()
{
    table->clear();
    table->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setVisible(false);
    table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    table->verticalHeader()->setVisible(false);
    table->setColumnCount(0);
    table->setRowCount(0);
}

void ListView::fill(QStringList col_file, QStringList col_prev)
{
    clean();

    int row_num = qMax(col_file.size(), col_prev.size());

    table->setColumnCount(2);
    table->setRowCount(row_num);

    for (int i = 0; i < row_num; i++)
    {
        QTableWidgetItem *item_file;
        QTableWidgetItem *item_prev;

        if (i < col_file.size())
            item_file = new QTableWidgetItem(col_file[i]);
        else
            item_file = new QTableWidgetItem("");

        if (i < col_prev.size())
            item_prev = new QTableWidgetItem(col_prev[i]);
        else
            item_prev = new QTableWidgetItem("");

        table->setItem(i, FILE_COL, item_file);
        table->setItem(i, PREVIEW_COL, item_prev);
    }
}


void ListView::fill(QStringList col)
{
    clean();

    if (col.empty())
        return;

    table->setColumnCount(2);
    table->setRowCount(col.length());

    for (int i = 0; i < col.length(); i++)
    {
        QTableWidgetItem *item0 = new QTableWidgetItem(col[i]);
        QTableWidgetItem *item1 = new QTableWidgetItem("");

        table->setItem(i, FILE_COL, item0);
        table->setItem(i, PREVIEW_COL, item1);
    }
}

void ListView::initIterator()
{
    count = 0;
}

bool ListView::hasNext()
{
    return (count < table->rowCount());
}

void ListView::next()
{
    count++;
}

void ListView::setFilePreview(QString text)
{
    table->item(count, PREVIEW_COL)->setText(text);
}

void ListView::setFile(QString text)
{
    table->item(count, FILE_COL)->setText(text);
}

QString ListView::getFile(void)
{
    QString file = table->item(count, FILE_COL)->text();
    return file;
}

QString ListView::getFilePreview(void)
{
    QString file = table->item(count, PREVIEW_COL)->text();
    return file;
}

void ListView::setRenamedOk(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/ok.png"));
    table->item(count, FILE_COL)->setText(table->item(count, PREVIEW_COL)->text());
    table->item(count, PREVIEW_COL)->setText("");
}


void ListView::setRenamedWarning(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/warning.png"));
}


void ListView::setRenamedError(void)
{
    table->item(count, FILE_COL)->setIcon(QIcon("./images/error.png"));
}

ListView::ListView(QTableWidget *t)
{
    table = t;
    clean();
};

ListView::~ListView()
{
}


Tweezers::Tweezers(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tweezers)
{
    preference_window = new Preference;
    tag = new TagConverter(preference_window);

    // Read settings
    resize(preference_window->getSize());
    move(preference_window->getPos());

    curr_path = preference_window->getPath();

    ui->setupUi(this);
    ui->selectDir->setText(curr_path);
    ui->globSelect->setText("*.*");
    ui->expField->setText(preference_window->getLastExp());
    ui->expList->addItems(tag->getTagDesc());

    table = new ListView(ui->fileList);
    timer = new QTimer(this);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setUnifiedTitleAndToolBarOnMac(true);

    // Some init actions
    loadFiles();
    // Update the preview only every 500ms
    timer->start(500);
}

Tweezers::~Tweezers()
{
    delete tag;
    delete preference_window;
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

 void Tweezers::closeEvent(QCloseEvent *e)
 {
    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"), tr("Do you want really exit?"),
               QMessageBox::Ok | QMessageBox::Cancel);

    if (ret == QMessageBox::Ok)
    {
        preference_window->setSize(size());
        preference_window->setPos(pos());
        preference_window->setPath(curr_path);
        preference_window->setLastExp(ui->expField->text());

        e->accept();
    }
    else /* ret == QMessageBox::Cancel */
    {
        e->ignore();
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

    table->fill(file_list);

    // Update preview if there is already written the expression into its field.
    if (!ui->expField->text().isEmpty())
    {
        expr_changed = true;
        preview();
    }
}

void Tweezers::exprTextMod()
{
    expr_changed = true;
}

void Tweezers::preview()
{
    if (!expr_changed)
        return;

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
    table->initIterator();
    while(table->hasNext())
    {
        QString value = tag->fill_tags(curr_path, table->getFile(), ui->expField->text(), tag_list);
        table->setFilePreview(value);
        table->next();
    }

    expr_changed = false;
}


void Tweezers::renameList(QList<QTableWidgetItem *>items)
{

}

void Tweezers::renameAll()
{
    int count_renamed = 0;
    int count_warning = 0;
    int count_error = 0;


    table->initIterator();
    while(table->hasNext())
    {
        QString origin = curr_path + QDir::separator () + table->getFile();
        QString renamed = curr_path + QDir::separator () + table->getFilePreview();

        QFile origin_filename(origin);

        // The are two file with same name, we skip it.
        if (backup.contains(renamed))
        {
            table->setRenamedWarning();
            table->next();
            count_warning++;
            continue;
        }

        if (origin_filename.rename(renamed))
        {
            backup[renamed] = origin;
            table->setRenamedOk();
            count_renamed++;
        }
        else /* Rename fail, set warning in the layout */
        {
            table->setRenamedError();
            count_error++;
        }

        statusBar()->showMessage(tr("Renamed: ") + QString::number(count_renamed) +
                                 tr(" Warning: ") +  QString::number(count_warning) +
                                 tr(" Error: ") + QString::number(count_error));
        table->next();
    }

    expr_changed = false;
}

void Tweezers::renameSelection()
{
    QList<QTableWidgetItem *> list;
    list = ui->fileList->selectedItems();

    for (int i = 0; i < list.size(); i++)
    {
        qDebug() << list[i]->text();
    }
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
    expr_changed = true;
    preview();
    statusBar()->showMessage("Ready");
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

void Tweezers::preferences()
{
    preference_window->show();
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

#ifdef Q_WS_MAC
    preferenceAct->setShortcuts(QKeySequence::Preferences);
#endif

    preferenceAct->setStatusTip(tr("Tweezers preference"));
    connect(preferenceAct, SIGNAL(triggered()), this, SLOT(preferences()));

    // About window action
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openDir()));

    exitAct = new QAction(tr("E&xit"), this);
#ifdef Q_WS_MAC
    exitAct->setShortcuts(QKeySequence::Quit);
#else
    exitAct->setShortcuts(QKeySequence::Close);
#endif
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    // Manage all directory widget signals
    connect(ui->selDirButton, SIGNAL(clicked()), this, SLOT(openDir()));
    connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(loadFiles()));
    connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));

    // Manage all expression field signals
    connect(ui->expField, SIGNAL(textChanged(const QString)), this, SLOT(exprTextMod()));
        connect(timer, SIGNAL(timeout()), this, SLOT(preview()));

    // Manage the rename action signals
    connect(ui->doRename, SIGNAL(clicked()), this, SLOT(renameAll()));
    connect(ui->undoRename, SIGNAL(clicked()), this, SLOT(undoRename()));

    // Manage tag list signals
    connect(ui->expList, SIGNAL(activated(int)), this, SLOT(selExpCombo(int)));

    // Manage list file to rename
    connect(ui->fileList, SIGNAL(itemSelectionChanged()), this, SLOT(renameSelection()));
}

void Tweezers::createMenus()
{
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(openAct);
     fileMenu->addSeparator();
     fileMenu->addAction(exitAct);

     editMenu = menuBar()->addMenu(tr("&Edit"));
     editMenu->addAction(preferenceAct);

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

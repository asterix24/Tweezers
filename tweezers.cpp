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
#include <QDir>
#include <iostream>
#include <fileinfo.h>

Tweezers::Tweezers(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::Tweezers)
{
	preference_window = new Preference;
	tag_converter = new TagConverter(preference_window);

	// Settings block
	resize(preference_window->getSize());
	move(preference_window->getPos());
	curr_path = preference_window->getPath();

	// Set default widget value
	ui->setupUi(this);
	ui->selectDir->setText(curr_path);
	ui->globSelect->setText("*.*");
	ui->expField->setText(preference_window->getLastExp());
	ui->expList->addItem(tr("Elenco dei tag.."));
	ui->expList->addItems(tag_converter->getDescriptionList());

	// Create layout object
	table = new ListView(ui->fileList, tag_converter);
	timer = new QTimer(this);

	createActions();
	createMenus();
	createToolBars();
	createStatusBar();

	setUnifiedTitleAndToolBarOnMac(true);

	// Init action
	loadFiles();
	table->preview(ui->expField->text());
	table->showFiles();
	statusBar()->showMessage(tr("Caricati: ") + QString::number(table->lenght()));


	// Update the preview lazy
	timer->start(200);
	expr_changed = false;
}

Tweezers::~Tweezers()
{
	delete preference_window;
	delete tag_converter;
	delete table;
	delete timer;
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

void Tweezers::openDirectory()
{
	curr_path = QFileDialog::getExistingDirectory(this, QString(tr("Open Directory")), curr_path, QFileDialog::ShowDirsOnly);

	if (! curr_path.isEmpty())
		ui->selectDir->setText(curr_path);
}

void Tweezers::selectDirectory()
{
	curr_path = ui->selectDir->text();

	if (!curr_path.isEmpty())
		updateFiles();
}

void Tweezers::filterView(void)
{
	updateFiles();
	table->preview(ui->expField->text());
	table->showFiles();
	statusBar()->showMessage(tr("Loaded: ") + QString::number(table->lenght()));
}

void Tweezers::loadFiles(void)
{
	QDir dir(curr_path);

	table->addFiles(curr_path, dir.entryList(QDir::Files,QDir::Name));
	table->showFiles();
	statusBar()->showMessage(tr("Loaded: ") + QString::number(table->lenght()));

	ui->expSelect->clear();
	ui->expSelect->addItem("*.*");
	ui->expSelect->addItems(table->extensionList());
}

void Tweezers::updateFiles()
{
	QDir dir(curr_path);

	QString glob = ui->globSelect->text();
	QStringList glob_list;

	if (glob.isEmpty())
		glob_list << "*.*";
	else
		glob_list << glob;

	table->addFiles(curr_path, dir.entryList(glob_list, QDir::Files));

	if (!ui->expField->text().isEmpty())
	{
		expr_changed = true;
		table->preview(ui->expField->text());
	}

	table->showFiles();
	statusBar()->showMessage(tr("Loaded: ") + QString::number(table->lenght()));
}

void Tweezers::expressionFieldChanged()
{
	expr_changed = true;
}

void Tweezers::preview()
{
	if (expr_changed && !ui->expField->text().isEmpty())
	{
		table->preview(ui->expField->text());
		table->showFiles();
		expr_changed = false;
	}
	else if (expr_changed)
	{
		table->preview(ui->expField->text());
		table->showFiles();
	}
}


void Tweezers::rename()
{
}

void Tweezers::renameSelection()
{
}

void Tweezers::undoRename()
{
	statusBar()->showMessage(tr("Ready"));
}

void Tweezers::fileInfoBox(int r, int c)
{
	(void)c;

	ItemNode node = table->item(r);
	FileInfo e(node.full_origin_name);
	QStringList l = e.allExifTag();
	for(int i = 0; i < l.size(); i++)
		ui->fileInfo->append(l[i]);
}

/*
 * Add to the expressiont line edit the tag select
 * from the expList combobox
 */
void Tweezers::tagSelectionList(int index)
{
	// Skip the decription text
	if (!index)
		return;

	QStringList list = ui->expList->itemText(index).split(":");

	if (!list.isEmpty())
		ui->expField->insert(list.at(0));
}

/*
 * Insert into the filter line edit the extension select
 * from combo list.
 */
void Tweezers::extensionSelectionList(int index)
{
	if (ui->expSelect->itemText(index) != "*.*")
		ui->globSelect->setText("*." + ui->expSelect->itemText(index));
	else
		ui->globSelect->setText("*.*");
	updateFiles();
}

void Tweezers::preferences()
{
	preference_window->show();
}

void Tweezers::about()
{
	QMessageBox::about(this, tr("About Application"), tr(""));
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
	connect(openAct, SIGNAL(triggered()), this, SLOT(openDirectory()));

	exitAct = new QAction(tr("E&xit"), this);
#ifdef Q_WS_MAC
	exitAct->setShortcuts(QKeySequence::Quit);
#else
	exitAct->setShortcuts(QKeySequence::Close);
#endif
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	// Manage all directory widget signals
	connect(ui->selDirButton, SIGNAL(clicked()), this, SLOT(openDirectory()));
	connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(filterView()));
	connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));

	// Manage all expression field signals
	connect(ui->expField, SIGNAL(textChanged(const QString)), this, SLOT(expressionFieldChanged()));
	connect(timer, SIGNAL(timeout()), this, SLOT(preview()));

	// Manage the rename action signals
	connect(ui->doRename, SIGNAL(clicked()), this, SLOT(rename()));
	connect(ui->undoRename, SIGNAL(clicked()), this, SLOT(undoRename()));

	// Manage tag list signals
	connect(ui->expList, SIGNAL(activated(int)), this, SLOT(tagSelectionList(int)));

	// Manage list file to rename
	connect(ui->fileList, SIGNAL(itemSelectionChanged()), this, SLOT(renameSelection()));
	connect(ui->fileList, SIGNAL(cellClicked(int,int)), this, SLOT(fileInfoBox(int, int)));
	// Manage ext selector
	connect(ui->expSelect, SIGNAL(activated(int)), this, SLOT(extensionSelectionList(int)));
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

#include <QtGui>

#include "tweezers.h"
#include "ui_tweezers.h"
#include "tag_converter.h"

Tweezers::Tweezers(QWidget *parent) :
    QMainWindow(parent),
    tag(),
    ui(new Ui::Tweezers)
{

    select_dir =  QDir::home().absolutePath();
    file_list.clear();

    ui->setupUi(this);
    ui->selectDir->setText(select_dir);
    ui->globSelect->setText("*.*");
    ui->expField->setText("<exiftime>");

    createActions();
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

void Tweezers::open()
{
    select_dir = QFileDialog::getExistingDirectory(this, QString(tr("Open Directory")), select_dir, QFileDialog::ShowDirsOnly);

    if (! select_dir.isEmpty())
        ui->selectDir->setText(select_dir);
}

void Tweezers::selectDirectory()
{
    select_dir = ui->selectDir->text();

    if (! select_dir.isEmpty())
        preview();
}

void Tweezers::preview()
{
    QDir dir(select_dir);

    glob_exp.clear();
    if (ui->globSelect->isModified())
        glob_exp << ui->globSelect->text();

    file_list.clear();
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

    QString expr = ui->expField->text();

    for (int i = 0; i < file_list.length(); i++)
    {   
        QTableWidgetItem *newItem = new QTableWidgetItem(file_list[i]);
        QTableWidgetItem *item_preview = new QTableWidgetItem(tag.fill_tags(select_dir, file_list[i], expr));
        ui->fileList->setItem(i, 0, newItem);
        ui->fileList->setItem(i, 1, item_preview);
    }
}

void Tweezers::cleanTable()
{

    ui->fileList->clear();
    ui->fileList->horizontalHeader()->setVisible(false);
    ui->fileList->setColumnCount(0);
    ui->fileList->setRowCount(0);
}

void Tweezers::createActions()
{
    connect(ui->selDirButton, SIGNAL(clicked()), this, SLOT(open()));
    connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(preview()));
    connect(ui->cleanAll, SIGNAL(clicked()), this, SLOT(cleanTable()));
    connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));
}

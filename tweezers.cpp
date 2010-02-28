#include <QtGui>

#include "tweezers.h"
#include "ui_tweezers.h"
#include "tag_converter.h"

Tweezers::Tweezers(QWidget *parent) :
    QMainWindow(parent),
    tag(),
    ui(new Ui::Tweezers)
{

    curr_path =  QDir::home().absolutePath();
    file_list.clear();

    ui->setupUi(this);
    ui->selectDir->setText(curr_path);
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
        preview();
}

void Tweezers::preview()
{
    QDir dir(curr_path);

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
        QTableWidgetItem *item_preview = new QTableWidgetItem(tag.fill_tags(curr_path, file_list[i], expr));
        ui->fileList->setItem(i, 0, newItem);
        ui->fileList->setItem(i, 1, item_preview);
    }
}

void Tweezers::renameAll()
{
    for (int i = 0; i < ui->fileList->rowCount(); i++)
    {
        QString old_name = curr_path + QDir::separator () + ui->fileList->item(i,0)->text();
        QString new_name = curr_path + QDir::separator () + ui->fileList->item(i,1)->text();

        QFile old_filename(old_name);
        QFile new_filename(new_name);

        if (backup.contains(new_name))
            continue;

        backup[old_name] = new_name;
        old_filename.rename(new_name);
    }

}

void Tweezers::renameSelection()
{
}

void Tweezers::undoRename()
{
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
    connect(ui->selDirButton, SIGNAL(clicked()), this, SLOT(openDir()));
    connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(preview()));
    connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));
    connect(ui->expField, SIGNAL(textChanged(const QString)), this, SLOT(preview()));
    connect(ui->doRename, SIGNAL(clicked()), this, SLOT(renameAll()));
    connect(ui->undoRename, SIGNAL(clicked()), this, SLOT(undoRename()));
}

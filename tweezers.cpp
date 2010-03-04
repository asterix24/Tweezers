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

    ui->setupUi(this);
    ui->selectDir->setText(curr_path);
    ui->globSelect->setText("*.*");
    ui->expField->setText("");

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
    for (int i = 0; i < ui->fileList->rowCount(); i++)
    {
        QString value = tag.fill_tags(curr_path, ui->fileList->item(i, FILE_COL)->text(), ui->expField->text());
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
    connect(ui->globSelect, SIGNAL(textChanged(const QString)), this, SLOT(loadFiles()));
    connect(ui->selectDir, SIGNAL(textChanged(const QString)), this, SLOT(selectDirectory()));
    connect(ui->expField, SIGNAL(textChanged(const QString)), this, SLOT(preview()));
    connect(ui->doRename, SIGNAL(clicked()), this, SLOT(renameAll()));
    connect(ui->undoRename, SIGNAL(clicked()), this, SLOT(undoRename()));
}

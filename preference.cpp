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
#include "preference.h"
#include "ui_preference.h"

#include <cfg/cfg_tweezers.h>

#include <QtGui>
#include <QDate>
#include <QDebug>

#define CATEGORY               1

enum
{
    DATE_FMT,
    TIME_FMT,

    CAT_CNT
};

typedef struct Category
{
    int index;
    QString label;
    QString fmt;
} Category;


Category cat[CAT_CNT] =
{
    { DATE_FMT, "Date", DEFAULT_DATE_FMT },
    { TIME_FMT, "Time", DEFAULT_TIME_FMT }
};



Preference::Preference(QWidget *parent) :
            QWidget(parent),
            m_ui(new Ui::Preference)
{
    m_ui->setupUi(this);
    createActions();
    readSettings();

    m_ui->categoryList->clear();
    m_ui->categoryList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_ui->categoryList->horizontalHeader()->setVisible(false);
    m_ui->categoryList->verticalHeader()->setVisible(false);
    m_ui->categoryList->setColumnCount(1);
    m_ui->categoryList->setRowCount(0);
    m_ui->saveOnExit->setCheckState(save_on_exit ? Qt::Checked : Qt::Unchecked);

    fillCategory();
}

Preference::~Preference()
{
    writeSettings();
    delete m_ui;
}

void Preference::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
            m_ui->retranslateUi(this);
            break;
    default:
            break;
    }
}

void Preference::ok(void)
{
    writeSettings();
    this->close();
}

void Preference::cancel(void)
{
    this->close();
}

void Preference::readSettings()
{
    QSettings settings("Asterix", "Tweezers application");
    pos = settings.value("pos", QPoint(200, 200)).toPoint();
    size = settings.value("size", QSize(400, 400)).toSize();

    // Last directory
    curr_path = settings.value("curr_path", QDir::home().absolutePath()).toString();
    last_expr = settings.value("expr", DEFAULT_EXPR).toString();

    // Last directory
    curr_path = settings.value("curr_path", QDir::home().absolutePath()).toString();
    last_expr = settings.value("expr", DEFAULT_EXPR).toString();

    // Format
    cat[DATE_FMT].fmt = settings.value("date_fmt", DEFAULT_DATE_FMT).toString();
    cat[TIME_FMT].fmt = settings.value("time_fmt", DEFAULT_TIME_FMT).toString();

    //Other settings
    save_on_exit = settings.value("save_on_exit", true).toBool();
}

void Preference::writeSettings()
{
    QSettings settings("Asterix", "Tweezers application");

    // Windows settings.
    settings.setValue("pos", pos);
    settings.setValue("size", size);

    // Last directory
    settings.setValue("curr_path", curr_path);
    settings.setValue("expr", last_expr);

    // Formats
    settings.setValue("date_fmt", cat[DATE_FMT].fmt);
    settings.setValue("time_fmt", cat[TIME_FMT].fmt);

    //Other settings
    settings.setValue("save_on_exit", save_on_exit);
}

void Preference::fillCategory()
{
    m_ui->categoryList->setRowCount(CAT_CNT);
    m_ui->categoryList->resizeRowsToContents();
    m_ui->categoryList->resizeColumnsToContents();

    for (int i = 0; i < CAT_CNT; i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(cat[i].label);
        QVariant d(cat[i].index);
        item->setData(CATEGORY, d);

        m_ui->categoryList->setItem(i, 0, item);
    }
}

void Preference::preview(QTableWidgetItem *item)
{
    QVariant index = item->data(CATEGORY);
    curr_fmt = index.toInt();
    QString fmt = cat[index.toInt()].fmt;
    m_ui->customFormat->setText(fmt);
    upDatePreview(fmt);
}

void Preference::upDatePreview(QString str)
{
    // Controllare quando la stringa non vale nulla..
    QDateTime now = QDateTime::currentDateTime();

    cat[curr_fmt].fmt = str;
    m_ui->preview->setText(now.toString(str));
}

void Preference::updateState()
{
    save_on_exit = m_ui->saveOnExit->isChecked();
}

QString Preference::getTimeFmt()
{
    return cat[TIME_FMT].fmt;
}

QString Preference::getDateFmt()
{
    return cat[DATE_FMT].fmt;
}

void Preference::createActions()
{
    // Manage all directory widget signals
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    // Manage tag list signals
    connect(m_ui->categoryList, SIGNAL(itemPressed(QTableWidgetItem *)), this, SLOT(preview(QTableWidgetItem *)));

    // Manage preview signals
    connect(m_ui->customFormat, SIGNAL(textChanged(const QString)), this, SLOT(upDatePreview(QString)));
    connect(m_ui->saveOnExit, SIGNAL(clicked()), this, SLOT(updateState()));
}


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

Preference::Preference(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Preference)
{
    m_ui->setupUi(this);
    createActions();
    readSettings();

    m_ui->formatList->clear();
    m_ui->formatList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_ui->formatList->horizontalHeader()->setVisible(false);
    m_ui->formatList->verticalHeader()->setVisible(false);
    m_ui->formatList->setColumnCount(1);
    m_ui->formatList->setRowCount(0);

    m_ui->categoryList->clear();
    m_ui->categoryList->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    m_ui->categoryList->horizontalHeader()->setVisible(false);
    m_ui->categoryList->verticalHeader()->setVisible(false);
    m_ui->categoryList->setColumnCount(1);
    m_ui->categoryList->setRowCount(0);



    QStringList date_fmt_lst;
    QStringList time_fmt_lst;

	date_fmt_lst << date_fmt << "dddd MMMM yyyy" << "d/m/yy" << "dd-mm-yy";
	time_fmt_lst << time_fmt << "hh:MM:ss" << "h:M" << "hh-MM-ss";

    category["Date"] = date_fmt_lst;
    category["Time"] = time_fmt_lst;

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

	//Format settings
	time_fmt = settings.value("time", DEFAULT_TIME_FMT).toString();
	date_fmt = settings.value("date", DEFAULT_DATE_FMT).toString();
}

void Preference::writeSettings()
{
	QSettings settings("Asterix", "Tweezers application");
	settings.setValue("pos", pos);
	settings.setValue("size", size);

	//Path settings and expression
	settings.setValue("curr_path", curr_path);
	settings.setValue("expr", last_expr);

	//Format settings
	settings.setValue("time", time_fmt);
	settings.setValue("date", date_fmt);
}

void Preference::fillCategory()
{
    m_ui->categoryList->setRowCount(category.size());
    m_ui->categoryList->resizeRowsToContents();
    m_ui->categoryList->resizeColumnsToContents();


    int j = 0;
    QHashIterator<QString, QStringList> i(category);
    while (i.hasNext())
    {
       i.next();
       QTableWidgetItem *item = new QTableWidgetItem(i.key());
       m_ui->categoryList->setItem(j, 0, item);
       j++;
    }
}

void Preference::fillFormat(QTableWidgetItem *item)
{
    if (category.contains(item->text()))
    {
        QStringList list = category[item->text()];

		//Ugly..
		m_ui->label_3->setText(item->text());
		if ((item->text() == "Date"))
		{
			upDatePreview(date_fmt);
		}
		else if ((item->text() == "Time"))
		{
			upDatePreview(time_fmt);
		}
		qDebug() << "Update " << item->text();

        // Some view adjustment
        m_ui->formatList->setRowCount(list.size());
        m_ui->formatList->resizeRowsToContents();
        m_ui->formatList->resizeColumnsToContents();

        // Mark all item whit its category.
        QVariant v(item->text());

        for (int i = 0; i < list.size(); i++)
        {
            QTableWidgetItem *_item = new QTableWidgetItem(list[i]);
            _item->setData(Qt::UserRole, v);
            m_ui->formatList->setItem(i, 0, _item);
        }
    }

}

void Preference::fillCustomFmt(QTableWidgetItem *item)
{
    QVariant d = item->data(Qt::UserRole);
    m_ui->label_3->setText(d.toString());
    qDebug() << d.toString();
    upDatePreview(item->text());
}

void Preference::upDatePreview(QString str)
{
    QDateTime now = QDateTime::currentDateTime();

    if ((m_ui->label_3->text() == "Date"))
	{
		date_fmt = str;
		m_ui->customFormat->setText(date_fmt);
        m_ui->preview->setText(now.toString(date_fmt));
	}
	else if ((m_ui->label_3->text() == "Time"))
    {
		time_fmt = str;
		m_ui->customFormat->setText(time_fmt);
		m_ui->preview->setText(now.toString(time_fmt));
    }
    else
    {
        m_ui->preview->setText("");
    }
}

void Preference::createActions()
{
    // Manage all directory widget signals
    connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));

    // Manage tag list signals
    connect(m_ui->categoryList, SIGNAL(itemPressed (QTableWidgetItem *)), this, SLOT(fillFormat(QTableWidgetItem *)));
    connect(m_ui->formatList, SIGNAL(itemPressed (QTableWidgetItem *)), this, SLOT(fillCustomFmt(QTableWidgetItem *)));

    // Manage preview signals
    connect(m_ui->customFormat, SIGNAL(textChanged(const QString)), this, SLOT(upDatePreview(QString)));
}


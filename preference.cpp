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

Preference::Preference(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Preference)
{
    m_ui->setupUi(this);
}

Preference::~Preference()
{
    delete m_ui;
}

void Preference::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Preference::ok(void)
{
    this->close();
}

void Preference::cancel(void)
{
}

void Preference::createActions()
{
    // Manage all directory widget signals
    connect(m_ui->buttonBox, SIGNAL(clicked()), this, SLOT(ok()));
}

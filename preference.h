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
#ifndef PREFERENCE_H
#define PREFERENCE_H

#include <QtGui/QWidget>

namespace Ui {
    class Preference;
}

class Preference : public QWidget {
    Q_OBJECT
public:
    Preference(QWidget *parent = 0);
    ~Preference();

protected:
    void changeEvent(QEvent *e);

private:
    void ok(void);
    void cancel(void);
    void createActions();

    Ui::Preference *m_ui;
};

#endif // PREFERENCE_H

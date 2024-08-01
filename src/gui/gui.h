/*************************************************************************
 * This file is part of tuna
 * git.vrsal.xyz/alex/tuna
 * Copyright 2023 univrsal <uni@vrsal.xyz>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#pragma once

#include <QDialog>
#include <map>
#include <tuple>

namespace Ui {
    class gui;
}

class source_widget : public QWidget {
    Q_OBJECT
public:
    explicit source_widget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
    }
    virtual void save_settings() = 0;
    virtual void load_settings() = 0;
    virtual void tick() { }
};

class gui : public QDialog {
    Q_OBJECT

    QList<source_widget*> m_source_widgets;
    QTimer* m_refresh = nullptr;

public:
    explicit gui(QWidget* parent = nullptr);

    ~gui();
private slots:

    Ui::gui* ui;
};

extern gui* dialog;
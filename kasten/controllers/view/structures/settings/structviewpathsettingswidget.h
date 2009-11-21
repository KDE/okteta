/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef STRUCTVIEWPATHSETTINGSWIDGET_H
#define STRUCTVIEWPATHSETTINGSWIDGET_H

#include <QtGui/QWidget>
#include "ui_structviewpathsettingswidget.h"

class StructViewPathSettingsWidget: public QWidget
{
Q_OBJECT
public:
    explicit StructViewPathSettingsWidget(QWidget *parent = 0);
    ~StructViewPathSettingsWidget();

private:
    Ui_StructViewPathSettingsWidget ui;
protected Q_SLOTS:
    void onAddFilesClicked();
    void onRemoveFilesClicked();
};

#endif // STRUCTVIEWPATHSETTINGSWIDGET_H

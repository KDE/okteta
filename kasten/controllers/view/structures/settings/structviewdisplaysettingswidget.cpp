/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
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
#include "structviewdisplaysettingswidget.h"
#include "structviewpreferences.h"

StructViewDisplaySettingsWidget::StructViewDisplaySettingsWidget() :
    QWidget(NULL)
{
    ui.setupUi(this);
    ui.combo_CharDisplayBase->setEnabled(ui.kcfg_ShowCharNumericalValue->isChecked());

    //no need for a hidden spinbox with byteOrder, since it is a simple sequential enum

    //setup the hidden spin boxes for the display bases
    //these are needed since KConfigXT always uses the combo box index as the value
    //we want the UserData of the current index instead
    //maybe there is a nicer solution, but this works
    ui.kcfg_CharDisplayBase->setValue(Kasten2::StructViewPreferences::charDisplayBase());
    ui.kcfg_CharDisplayBase->setHidden(true);
    ui.kcfg_SignedDisplayBase->setValue(Kasten2::StructViewPreferences::signedDisplayBase());
    ui.kcfg_SignedDisplayBase->setHidden(true);
    ui.kcfg_UnsignedDisplayBase->setValue(Kasten2::StructViewPreferences::unsignedDisplayBase());
    ui.kcfg_UnsignedDisplayBase->setHidden(true);
    setupBasesCombo(ui.combo_SignedDisplayBase, Kasten2::StructViewPreferences::self()->signedDisplayBaseItem(),
        Kasten2::StructViewPreferences::signedDisplayBase(), SLOT(setSignedDisplay(int)));
    setupBasesCombo(ui.combo_UnsignedDisplayBase, Kasten2::StructViewPreferences::self()->unsignedDisplayBaseItem(),
        Kasten2::StructViewPreferences::unsignedDisplayBase(), SLOT(setUnsignedDisplay(int)));
    setupBasesCombo(ui.combo_CharDisplayBase, Kasten2::StructViewPreferences::self()->charDisplayBaseItem(),
        Kasten2::StructViewPreferences::charDisplayBase(), SLOT(setCharDisplay(int)));
}

StructViewDisplaySettingsWidget::~StructViewDisplaySettingsWidget()
{
}

void StructViewDisplaySettingsWidget::setupBasesCombo(QComboBox* box, KConfigSkeletonItem* configItem,
        int currentValue, const char* slot)
{
    Q_ASSERT(box->count() == 0);
    Q_ASSERT(currentValue == 2 || currentValue == 8 || currentValue == 10 || currentValue == 16);
    qDebug() << "current value:" << configItem->property() << "vs" << currentValue;
    box->addItem(i18nc("@item:inlistbox", "Binary"), 2);
    box->addItem(i18nc("@item:inlistbox", "Octal"), 8);
    box->addItem(i18nc("@item:inlistbox", "Decimal"), 10);
    box->addItem(i18nc("@item:inlistbox", "Hexadecimal"), 16);

    box->setCurrentIndex(currentValue == 2 ? 0 : (currentValue == 8 ? 1 : (currentValue == 16 ? 3 : 2)));

    box->setToolTip(configItem->toolTip());
    connect(box, SIGNAL(activated(int)), this, slot);
}

void StructViewDisplaySettingsWidget::handleMapping(int index, QComboBox* box, QSpinBox* spin)
{
    QVariant currentValue = box->itemData(index);
    qDebug() << "box changed to " << index << "value = " << currentValue;
    if (spin->value() != currentValue.toInt())
        spin->setValue(currentValue.toInt());
}

void StructViewDisplaySettingsWidget::setCharDisplay(int index)
{
    qDebug() << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_CharDisplayBase, ui.kcfg_CharDisplayBase);
}

void StructViewDisplaySettingsWidget::setSignedDisplay(int index)
{
    qDebug() << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_SignedDisplayBase, ui.kcfg_SignedDisplayBase);
}

void StructViewDisplaySettingsWidget::setUnsignedDisplay(int index)
{
    qDebug() << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_UnsignedDisplayBase, ui.kcfg_UnsignedDisplayBase);
}


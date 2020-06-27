/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureviewsettingswidget.hpp"

#include "structureviewpreferences.h"
#include "../structlogging.hpp"

#include <KLocalizedString>

StructureViewSettingsWidget::StructureViewSettingsWidget()
{
    ui.setupUi(this);
    ui.combo_CharDisplayBase->setEnabled(ui.kcfg_ShowCharNumericalValue->isChecked());

    // no need for a hidden spinbox with byteOrder, since it is a simple sequential enum

    // setup the hidden spin boxes for the display bases
    // these are needed since KConfigXT always uses the combo box index as the value
    // we want the UserData of the current index instead
    // maybe there is a nicer solution, but this works
    ui.kcfg_CharDisplayBase->setValue(Kasten::StructureViewPreferences::charDisplayBase());
    ui.kcfg_CharDisplayBase->setHidden(true);
    ui.kcfg_SignedDisplayBase->setValue(Kasten::StructureViewPreferences::signedDisplayBase());
    ui.kcfg_SignedDisplayBase->setHidden(true);
    ui.kcfg_UnsignedDisplayBase->setValue(Kasten::StructureViewPreferences::unsignedDisplayBase());
    ui.kcfg_UnsignedDisplayBase->setHidden(true);
    setupBasesCombo(ui.combo_SignedDisplayBase, Kasten::StructureViewPreferences::self()->signedDisplayBaseItem(),
                    Kasten::StructureViewPreferences::signedDisplayBase(), SLOT(setSignedDisplay(int)));
    setupBasesCombo(ui.combo_UnsignedDisplayBase, Kasten::StructureViewPreferences::self()->unsignedDisplayBaseItem(),
                    Kasten::StructureViewPreferences::unsignedDisplayBase(), SLOT(setUnsignedDisplay(int)));
    setupBasesCombo(ui.combo_CharDisplayBase, Kasten::StructureViewPreferences::self()->charDisplayBaseItem(),
                    Kasten::StructureViewPreferences::charDisplayBase(), SLOT(setCharDisplay(int)));
}

StructureViewSettingsWidget::~StructureViewSettingsWidget() = default;

void StructureViewSettingsWidget::setupBasesCombo(QComboBox* box, KConfigSkeletonItem* configItem,
                                                  int currentValue, const char* slot)
{
    Q_ASSERT(box->count() == 0);
    Q_ASSERT(currentValue == 2 || currentValue == 8 || currentValue == 10 || currentValue == 16);
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "current value:" << configItem->property() << "vs" << currentValue;
    box->addItem(i18nc("@item:inlistbox", "Binary"), 2);
    box->addItem(i18nc("@item:inlistbox", "Octal"), 8);
    box->addItem(i18nc("@item:inlistbox", "Decimal"), 10);
    box->addItem(i18nc("@item:inlistbox", "Hexadecimal"), 16);

    box->setCurrentIndex(currentValue == 2 ? 0 : (currentValue == 8 ? 1 : (currentValue == 16 ? 3 : 2)));

    box->setToolTip(configItem->toolTip());
    connect(box, SIGNAL(activated(int)), this, slot);
}

void StructureViewSettingsWidget::handleMapping(int index, QComboBox* box, QSpinBox* spin)
{
    QVariant currentValue = box->itemData(index);
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "box changed to " << index << "value = " << currentValue;
    if (spin->value() != currentValue.toInt()) {
        spin->setValue(currentValue.toInt());
    }
}

void StructureViewSettingsWidget::setCharDisplay(int index)
{
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_CharDisplayBase, ui.kcfg_CharDisplayBase);
}

void StructureViewSettingsWidget::setSignedDisplay(int index)
{
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_SignedDisplayBase, ui.kcfg_SignedDisplayBase);
}

void StructureViewSettingsWidget::setUnsignedDisplay(int index)
{
    qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "byteOrder changed to " << index;
    handleMapping(index, ui.combo_UnsignedDisplayBase, ui.kcfg_UnsignedDisplayBase);
}

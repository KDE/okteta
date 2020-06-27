/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "boolbitfielddatainformation.hpp"

#include <KLocalizedString>
#include <KComboBox>

#include <QScriptValue>

#include "../booldatainformation.hpp"
#include "../../../../poddecoder/typeeditors/uintspinbox.hpp"

QString BoolBitfieldDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    return BoolDataInformationMethods<quint64>::staticValueString(mValue.value<quint64>());
}

QWidget* BoolBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    if (width() == 1) {
        // just a simple combobox
        auto* box = new KComboBox(false, parent);
        box->addItem(i18nc("boolean value", "false"));
        box->addItem(i18nc("boolean value", "true"));
        box->setCurrentIndex((mValue.value<quint64>() != 0) ? 1 : 0);
        return box;
    }
    auto* ret = new UIntSpinBox(parent);
    ret->setBase(Kasten::StructureViewPreferences::unsignedDisplayBase());
    ret->setMaximum(mask());
    return ret;
}

QVariant BoolBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    if (width() == 1) {
        const auto* box = qobject_cast<const KComboBox*>(w);
        Q_CHECK_PTR(box);
        return box->currentIndex();
    }
    const auto* spin = qobject_cast<const UIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin) {
        return spin->value();
    }
    return {};
}

void BoolBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    auto* spin = qobject_cast<UIntSpinBox*> (w);
    if (spin) {
        spin->setValue(mValue.value<quint64>());
    }
}

QScriptValue BoolBitfieldDataInformation::valueAsQScriptValue() const
{
    return (mValue.value<quint64>()) != 0;
}

QString BoolBitfieldDataInformation::typeNameImpl() const
{
    return i18ncp("Data type", "boolean bitfield (%1 bit wide)", "boolean bitfield (%1 bits wide)",
                  width());
}

AbstractBitfieldDataInformation::Type BoolBitfieldDataInformation::bitfieldType() const
{
    return AbstractBitfieldDataInformation::Type::Boolean;
}

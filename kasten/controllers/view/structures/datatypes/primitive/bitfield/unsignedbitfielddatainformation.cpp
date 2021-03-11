/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "unsignedbitfielddatainformation.hpp"

#include <KLocalizedString>

#include <QScriptValue>

#include <view/poddecoder/typeeditors/uintspinbox.hpp>
#include "../uintdatainformation.hpp"

QString UnsignedBitfieldDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    return UIntDataInformationMethods<quint64>::staticValueString(mValue.value<quint64>());
}

QWidget* UnsignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    auto* ret = new UIntSpinBox(parent);
    ret->setBase(Kasten::StructureViewPreferences::unsignedDisplayBase());
    ret->setMaximum(mask());
    return ret;
}

QVariant UnsignedBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    const auto* spin = qobject_cast<const UIntSpinBox*> (w);
    if (spin) {
        return spin->value();
    }
    return {};
}

void UnsignedBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    auto* spin = qobject_cast<UIntSpinBox*> (w);
    if (spin) {
        spin->setValue(mValue.value<quint64>());
    }
}

QScriptValue UnsignedBitfieldDataInformation::valueAsQScriptValue() const
{
    if (width() <= 32) {
        return mValue.value<quint32>() & quint32(mask());  // 32 bit or less -> can be put in as value
    }

    return QString::number(mValue.value<quint64>());
}

QString UnsignedBitfieldDataInformation::typeNameImpl() const
{
    return i18ncp("Data type", "unsigned bitfield (%1 bit wide)",
                  "unsigned bitfield (%1 bits wide)", width());
}

AbstractBitfieldDataInformation::Type UnsignedBitfieldDataInformation::bitfieldType() const
{
    return AbstractBitfieldDataInformation::Type::Unsigned;
}

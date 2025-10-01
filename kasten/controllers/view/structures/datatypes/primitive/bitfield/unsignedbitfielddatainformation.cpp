/*
    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "unsignedbitfielddatainformation.hpp"

// controller
#include <uintdatainformation.hpp>
// libdatatypeeditors
#include <uintspinbox.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QScriptValue>

QString UnsignedBitfieldDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    return UIntDataInformationMethods<quint64>::staticValueString(mValue.value<quint64>());
}

QVariant UnsignedBitfieldDataInformation::valueToQVariant() const
{
    Q_ASSERT(mWasAbleToRead);
    return UIntDataInformationMethods<quint64>::staticToQVariant(mValue.value<quint64>());
}

QString UnsignedBitfieldDataInformation::valueToQString(AllPrimitiveTypes value) const
{
    return UIntDataInformationMethods<quint64>::staticValueString(value.value<quint64>());
}

QVariant UnsignedBitfieldDataInformation::valueToQVariant(AllPrimitiveTypes value) const
{
    return UIntDataInformationMethods<quint64>::staticToQVariant(value.value<quint64>());
}

QWidget* UnsignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    auto* const ret = new Okteta::UIntSpinBox(parent);
    ret->setBase(Kasten::StructureViewPreferences::unsignedDisplayBase());
    ret->setMaximum(mask());
    return ret;
}

QVariant UnsignedBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    const auto* const spin = qobject_cast<const Okteta::UIntSpinBox*>(w);
    if (spin) {
        return spin->value();
    }
    return {};
}

void UnsignedBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    auto* const spin = qobject_cast<Okteta::UIntSpinBox*>(w);
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

/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "signedbitfielddatainformation.hpp"

#include <KLocalizedString>

#include <QScriptValue>

#include <view/poddecoder/typeeditors/sintspinbox.hpp>
#include "../sintdatainformation.hpp"

QString SignedBitfieldDataInformation::valueStringImpl() const
{
    Q_ASSERT(mWasAbleToRead);
    return SIntDataInformationMethods<qint64>::staticValueString(mValue.value<qint64>());
}

QWidget* SignedBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    auto* ret = new SIntSpinBox(parent);
    ret->setBase(Kasten::StructureViewPreferences::signedDisplayBase());
    ret->setRange(~mask(), mask() >> 1); // mask is unsigned, so shift will do the right thing
    return ret;
}

QVariant SignedBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    const auto* spin = qobject_cast<const SIntSpinBox*> (w);
    if (spin) {
        return spin->value();
    }

    return {};
}

void SignedBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    auto* spin = qobject_cast<SIntSpinBox*> (w);
    if (spin) {
        spin->setValue(mValue.value<qint64>());
    }
}

QScriptValue SignedBitfieldDataInformation::valueAsQScriptValue() const
{
    if (width() <= 32) {
        return qint32(mValue.value<qint32>());  // 32 bit or less -> can be put in as value
    }
    // have to save it as string since 64 bit values are not supported
    return QString::number(mValue.value<qint64>());
}

void SignedBitfieldDataInformation::setValue(AllPrimitiveTypes newVal)
{
    // check that values are not too large
    Q_ASSERT((newVal.value<qint64>() < 0 && (newVal.value<qint64>() | ~mask()) == newVal.value<quint64>())
             || (newVal.value<quint64>() & mask()) == newVal.value<quint64>());
    mValue = newVal.value<quint64>() & mask();
    // check if MSB is set -> negative -> sign extend
    if (newVal.value<quint64>() & (quint64(1) << (width() - 1))) {
        mValue = mValue.value<quint64>() | (~mask());
    }
}

AllPrimitiveTypes SignedBitfieldDataInformation::fromVariant(const QVariant& variant, bool* ok) const
{
    return AllPrimitiveTypes(variant.toLongLong(ok));
}

QString SignedBitfieldDataInformation::typeNameImpl() const
{
    return i18ncp("Data type", "signed bitfield (%1 bit wide)", "signed bitfield (%1 bits wide)",
                  width());
}

AbstractBitfieldDataInformation::Type SignedBitfieldDataInformation::bitfieldType() const
{
    return AbstractBitfieldDataInformation::Type::Signed;
}

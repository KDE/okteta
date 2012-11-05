/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "boolbitfielddatainformation.h"

#include <KComboBox>
#include <QScriptValue>

#include "../booldatainformation.h"
#include "../../../../poddecoder/typeeditors/uintspinbox.h"


QString BoolBitfieldDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    return BoolDataInformationMethods<quint64>::staticValueString(mValue.ulongValue);
}

QWidget* BoolBitfieldDataInformation::createEditWidget(QWidget* parent) const
{
    if (width() == 1)
    {
        //just a simple combobox
        KComboBox* box = new KComboBox(false, parent);
        box->addItem(i18nc("boolean value", "false"));
        box->addItem(i18nc("boolean value", "true"));
        box->setCurrentIndex(mValue.ubyteValue ? 1 : 0);
        return box;
    }
    UIntSpinBox* ret = new UIntSpinBox(parent);
    ret->setBase(PrimitiveDataInformation::unsignedDisplayBase());
    ret->setMaximum(mask());
    return ret;
}

QVariant BoolBitfieldDataInformation::dataFromWidget(const QWidget* w) const
{
    if (width() == 1)
    {
        const KComboBox* box = dynamic_cast<const KComboBox*>(w);
        Q_CHECK_PTR(box);
        return box->currentIndex();
    }
    const UIntSpinBox* spin = dynamic_cast<const UIntSpinBox*>(w);
    Q_CHECK_PTR(spin);
    if (spin)
        return spin->value();
    else
        return QVariant();
}

void BoolBitfieldDataInformation::setWidgetData(QWidget* w) const
{
    UIntSpinBox* spin = dynamic_cast<UIntSpinBox*> (w);
    if (spin)
        spin->setValue(mValue.ulongValue);
}

QScriptValue BoolBitfieldDataInformation::valueAsQScriptValue() const
{
    return (mValue.ulongValue) != 0;
}


/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP
#define KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP

#include "abstractbitfielddatainformation.hpp"

class UnsignedBitfieldDataInformation : public AbstractBitfieldDataInformation
{
    DATAINFORMATION_CLONE(UnsignedBitfieldDataInformation, AbstractBitfieldDataInformation) {}

public:
    UnsignedBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr)
        : AbstractBitfieldDataInformation(name, width, parent)
    {}
    ~UnsignedBitfieldDataInformation() override = default;

    QVariant valueToQVariant() const override;
    QScriptValue valueAsQScriptValue() const override;
    QString valueToQString(AllPrimitiveTypes value) const override;
    QVariant valueToQVariant(AllPrimitiveTypes value) const override;

    QWidget* createEditWidget(QWidget* parent) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;
    Type bitfieldType() const override;

private:
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;
};

#endif /* KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP */

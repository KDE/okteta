/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BOOLBITFIELDDATAINFORMATION_HPP
#define KASTEN_BOOLBITFIELDDATAINFORMATION_HPP

#include "abstractbitfielddatainformation.hpp"

class BoolBitfieldDataInformation : public AbstractBitfieldDataInformation
{
    DATAINFORMATION_CLONE(BoolBitfieldDataInformation, AbstractBitfieldDataInformation) {}

public:
    BoolBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr)
        : AbstractBitfieldDataInformation(name, width, parent)
    {}
    ~BoolBitfieldDataInformation() override = default;

    QScriptValue valueAsQScriptValue() const override;
    QWidget* createEditWidget(QWidget* parent) const override;
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;
    Type bitfieldType() const override;

private:
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;
};

#endif /* KASTEN_BOOLBITFIELDDATAINFORMATION_HPP */

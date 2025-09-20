/*
    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP
#define KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP

// lib
#include "abstractbitfielddatainformation.hpp"

class UnsignedBitfieldDataInformation : public AbstractBitfieldDataInformation
{
    DATAINFORMATION_CLONE(UnsignedBitfieldDataInformation, AbstractBitfieldDataInformation) {}

public:
    UnsignedBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr)
        : AbstractBitfieldDataInformation(name, width, parent)
    {}
    ~UnsignedBitfieldDataInformation() override = default;

public: // AbstractBitfieldDataInformation API
    [[nodiscard]]
    Type bitfieldType() const override;

public: // PrimitiveDataInformation API
    [[nodiscard]]
    QVariant valueToQVariant() const override;
    [[nodiscard]]
    QScriptValue valueAsQScriptValue() const override;
    [[nodiscard]]
    QString valueToQString(AllPrimitiveTypes value) const override;
    [[nodiscard]]
    QVariant valueToQVariant(AllPrimitiveTypes value) const override;

public: // DataInformation API
    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
};

#endif /* KASTEN_UNSIGNEDBITFIELDDATAINFORMATION_HPP */

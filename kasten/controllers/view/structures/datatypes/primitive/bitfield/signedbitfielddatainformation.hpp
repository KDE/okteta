/*
    SPDX-FileCopyrightText: 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SIGNEDBITFIELDDATAINFORMATION_HPP
#define KASTEN_SIGNEDBITFIELDDATAINFORMATION_HPP

// lib
#include "abstractbitfielddatainformation.hpp"

class SignedBitfieldDataInformation : public AbstractBitfieldDataInformation
{
    DATAINFORMATION_CLONE(SignedBitfieldDataInformation, AbstractBitfieldDataInformation) {}

public:
    SignedBitfieldDataInformation(const QString& name, BitCount32 width, DataInformation* parent = nullptr)
        : AbstractBitfieldDataInformation(name, width, parent)
    {}
    ~SignedBitfieldDataInformation() override = default;

public: // AbstractBitfieldDataInformation API
    [[nodiscard]]
    Type bitfieldType() const override;

public: // PrimitiveDataInformation API
    /** sign extend the value if it is negative, so it can always be treated as a plain qint64 */
    void setValue(AllPrimitiveTypes newVal) override;
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

protected: // AbstractBitfieldDataInformation API
    [[nodiscard]]
    AllPrimitiveTypes fromVariant(const QVariant& variant, bool* ok) const override;

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
};

#endif /* KASTEN_SIGNEDBITFIELDDATAINFORMATION_HPP */

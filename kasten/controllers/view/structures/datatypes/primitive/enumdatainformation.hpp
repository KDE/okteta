/*
    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDATAINFORMATION_HPP
#define KASTEN_ENUMDATAINFORMATION_HPP

// lib
#include "primitivedatainformation.hpp"
#include "enumdefinition.hpp"

class EnumDataInformation : public PrimitiveDataInformationWrapper
{
    DATAINFORMATION_CLONE(EnumDataInformation, PrimitiveDataInformationWrapper), mEnum(d.mEnum) {}

public:
    /** this object takes ownership of @p type */
    EnumDataInformation(const QString& name, PrimitiveDataInformation* type,
                        const EnumDefinition::Ptr& enumDef, DataInformation* parent = nullptr);
    ~EnumDataInformation() override;

public: // DataInformation API
    [[nodiscard]]
    QWidget* createEditWidget(QWidget* parent) const override;
    [[nodiscard]]
    QVariant dataFromWidget(const QWidget* w) const override;
    void setWidgetData(QWidget* w) const override;

public: // DataInformationBase API
    [[nodiscard]]
    bool isEnum() const override;

public:
    [[nodiscard]]
    EnumDefinition::Ptr enumValues() const;
    void setEnumValues(const QMap<AllPrimitiveTypes, QString>& newValues);

private: // DataInformation API
    [[nodiscard]]
    QString typeNameImpl() const override;
    [[nodiscard]]
    QString valueStringImpl() const override;
    [[nodiscard]]
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;

protected:
    EnumDefinition::Ptr mEnum;
};

inline EnumDefinition::Ptr EnumDataInformation::enumValues() const
{
    return mEnum;
}

inline void EnumDataInformation::setEnumValues(const QMap<AllPrimitiveTypes, QString>& newValues)
{
    mEnum->setValues(newValues);
}
#endif /* KASTEN_ENUMDATAINFORMATION_HPP */

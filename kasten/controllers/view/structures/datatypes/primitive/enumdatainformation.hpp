/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ENUMDATAINFORMATION_HPP
#define KASTEN_ENUMDATAINFORMATION_HPP

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

    bool isEnum() const override;

    EnumDefinition::Ptr enumValues() const;
    void setEnumValues(const QMap<AllPrimitiveTypes, QString>& newValues);

private:
    QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const override;
    QString valueStringImpl() const override;
    QString typeNameImpl() const override;

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

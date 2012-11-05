/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ENUMDATAINFORMATION_H_
#define ENUMDATAINFORMATION_H_

#include "primitivedatainformation.h"
#include "enumdefinition.h"

class EnumDataInformation: public PrimitiveDataInformationWrapper
{
    DATAINFORMATION_CLONE(EnumDataInformation, PrimitiveDataInformationWrapper), mEnum(d.mEnum) {}
public:
    /** this object takes ownership of @p type */
    EnumDataInformation(const QString& name, PrimitiveDataInformation* type,
            EnumDefinition::Ptr enumDef, DataInformation* parent = 0);
    virtual ~EnumDataInformation();

    virtual QString valueString() const;
    virtual QString typeName() const;
    virtual bool isEnum() const;

    EnumDefinition::Ptr enumValues() const;
    void setEnumValues(QMap<AllPrimitiveTypes, QString> newValues);
protected:
    virtual QScriptClass* scriptClass(ScriptHandlerInfo* handlerInfo) const;
protected:
    EnumDefinition::Ptr mEnum;
};

inline bool EnumDataInformation::isEnum() const
{
    return true;
}

inline EnumDefinition::Ptr EnumDataInformation::enumValues() const
{
    return mEnum;
}

inline void EnumDataInformation::setEnumValues(QMap<AllPrimitiveTypes, QString> newValues)
{
    mEnum->setValues(newValues);
}
#endif /* ENUMDATAINFORMATION_H_ */

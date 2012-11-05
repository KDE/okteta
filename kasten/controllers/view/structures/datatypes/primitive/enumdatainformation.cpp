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

#include "enumdatainformation.h"
#include "../topleveldatainformation.h"

#include <KLocalizedString>
#include <QScriptValue>

#include "../../script/classes/enumscriptclass.h"
#include "../../script/scriptlogger.h"

EnumDataInformation::EnumDataInformation(const QString& name, PrimitiveDataInformation* type,
        EnumDefinition::Ptr enumDef, DataInformation* parent)
        : PrimitiveDataInformationWrapper(name, type, parent), mEnum(enumDef)
{
    Q_CHECK_PTR(type);
    if (enumDef->type() != type->type())
        logWarn() << "incompatible types in definition and value: "
                << enumDef->type() << "and " << type->type();
    mValue->setParent(this);
}

EnumDataInformation::~EnumDataInformation() {}

QString EnumDataInformation::valueString() const
{
    Q_ASSERT(mWasAbleToRead);
    QString enumVal = mEnum->value(mValue->value());
    if (!enumVal.isEmpty())
    {
        return i18n("%1 (%2)", enumVal, mValue->valueString());
    }
    else
        return i18n("%1 (value not in enum)", mValue->valueString());
}

QString EnumDataInformation::typeName() const
{
    return i18n("enum (%1)", mValue->typeName());
}

QScriptClass* EnumDataInformation::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mEnumClass.data();
}

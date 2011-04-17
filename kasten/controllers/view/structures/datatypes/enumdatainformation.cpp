/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#include "topleveldatainformation.h"

#include "../script/scriptutils.h"
#include <QtScript/QScriptContext>
#include <QtScript/QScriptEngine>
#include <KDebug>

EnumDataInformation::EnumDataInformation(QString name, PrimitiveDataInformation* type,
            EnumDefinition::Ptr enumDef, DataInformation* parent) :
    PrimitiveDataInformation(name, parent), mEnum(enumDef), mValue(type)
{
    Q_CHECK_PTR(type);
    if (enumDef->type() != type->type())
        kWarning() << "incompatible types in definition and value: "
                << enumDef->type() << "and " << type->type();
    mValue->setParent(this);
    connect(mValue, SIGNAL(dataChanged()), SIGNAL(dataChanged()));
}

EnumDataInformation::EnumDataInformation(const EnumDataInformation& e) :
    PrimitiveDataInformation(e), mEnum(e.mEnum)
{
    mValue = static_cast<PrimitiveDataInformation*> (e.mValue->clone());
    mValue->setParent(this);
    connect(mValue, SIGNAL(dataChanged()), this, SIGNAL(dataChanged()));
}

EnumDataInformation::~EnumDataInformation()
{
    delete mValue;
}

QString EnumDataInformation::valueString() const
{
    if (!mWasAbleToRead)
        return mValue->valueString();
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
    return "enum";
}
QString EnumDataInformation::getTypeString() const
{
    return i18n("enum (%1)", mValue->typeName());
}

bool EnumDataInformation::setData(const QVariant& value, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, Okteta::Address address,
        quint64 bitsRemaining, quint8* bitOffset)
{
    if (this != inf)
        return false;
    //correct object -> use mValue as parameter so that PrimitiveDataInformation::setData() returns true
    //endianess will also be correct since it is initialized to Inherit and will then use this endianess
    bool ret = mValue->setData(value, mValue, out, address, bitsRemaining, bitOffset);
    return ret;
}
qint64 EnumDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    //update enum first (it is possible to change the enum definition this enum uses
    topLevelDataInformation()->updateElement(this);
    qint64 retVal = mValue->readData(input, address, bitsRemaining, bitOffset);
    mWasAbleToRead = retVal >= 0; //not able to read if mValue->readData returns -1
    return retVal;
}

QWidget* EnumDataInformation::createEditWidget(QWidget* parent) const
{
    return mValue->createEditWidget(parent);
}

QVariant EnumDataInformation::dataFromWidget(const QWidget* w) const
{
    return mValue->dataFromWidget(w);
}

void EnumDataInformation::setWidgetData(QWidget* w) const
{
    mValue->setWidgetData(w);
}

AllPrimitiveTypes EnumDataInformation::qVariantToAllPrimitiveTypes(
        const QVariant& value) const
{
    return mValue->qVariantToAllPrimitiveTypes(value);
}

QScriptValue EnumDataInformation::scriptValue() const
{
    //XXX for some reason forwarding to mValue does not work, instead just copy the code
    QScriptEngine* eng = engine();
    if (!eng)
        return QScriptValue();
    if (!mWasAbleToRead)
    {
        if (context())
            return context()->throwError("Attempting to read value from element"
                " that has not been read yet: " + name());
        else
            return QScriptValue("fail");
    }
    QScriptValue wrapObj = eng->newObject();
    ScriptUtils::object()->wrapAllPrimitiveTypes(wrapObj, mValue->value(),
            mValue->type());
    return wrapObj;
}

AllPrimitiveTypes EnumDataInformation::value() const
{
    return mValue->value();
}

void EnumDataInformation::setValue(AllPrimitiveTypes newVal)
{
    mValue->setValue(newVal);
}

/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2011  Alex Richardson <alex.richardson@gmx.de>
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
#include "primitivedatainformation.h"
#include "../datainformation.h"
#include "../topleveldatainformation.h"
#include "../../script/scriptutils.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/classes/primitivescriptclass.h"


#include <QtScript/QScriptEngine>

bool PrimitiveDataInformation::setData(const QVariant& valueVariant, Okteta::AbstractByteArrayModel *out,
        Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset)
{
    AllPrimitiveTypes oldVal(value());
    // this is implemented in the subclasses
    AllPrimitiveTypes valToWrite = qVariantToAllPrimitiveTypes(valueVariant);
    AllPrimitiveTypes newVal(oldVal);
    //this handles remaining < size() for us
    bool wasAbleToWrite = newVal.writeBits(size(), valToWrite, out, effectiveByteOrder(), address, bitsRemaining, &bitOffset);
    return wasAbleToWrite;
}

bool PrimitiveDataInformation::setChildData(uint, const QVariant&, Okteta::AbstractByteArrayModel*,
        Okteta::Address, BitCount64, quint8)
{
    Q_ASSERT_X(false, "PrimitiveDataInformation::setChildData()", "this should never be called!!");
    return false;
}


Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

qint64 PrimitiveDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    topLevelDataInformation()->updateElement(this);
    if (bitsRemaining < BitCount64(size()))
    {
        mWasAbleToRead = false;
        setValue(0);
        return -1;
    }
    bool wasValid = mWasAbleToRead;
    AllPrimitiveTypes oldVal(value());
    AllPrimitiveTypes newVal(value());

    mWasAbleToRead = newVal.readBits(size(), input, effectiveByteOrder(), address, bitsRemaining, bitOffset);

    if (oldVal != newVal || wasValid != mWasAbleToRead) {
        topLevelDataInformation()->setChildDataChanged();
        setValue(newVal);
    }

    return size();
}

PrimitiveDataInformation::PrimitiveDataInformation(QString name, DataInformation* parent) :
    DataInformation(name, parent)
{
}

PrimitiveDataInformation::PrimitiveDataInformation(const PrimitiveDataInformation& d) :
    DataInformation(d)
{
}

PrimitiveDataInformation::~PrimitiveDataInformation()
{
}

QScriptValue PrimitiveDataInformation::toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mPrimitiveClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

QString PrimitiveDataInformation::typeName(PrimitiveDataType type)
{
    Q_ASSERT(type >= Type_START && type <= Type_END);

    if (Kasten2::StructViewPreferences::shortTypeNames())
        return PrimitiveType::typeNames[type];
    else
        return i18n(PrimitiveType::longTypeNames[type]);
}

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
#include "../../script/classes/primitivescriptclass.h"


#include <QtScript/QScriptEngine>

bool PrimitiveDataInformation::setData(const QVariant& valueVariant, DataInformation* inf,
        Okteta::AbstractByteArrayModel *out, Okteta::Address address,
        quint64 bitsRemaining, quint8* bitOffset)
{
    if (this != inf)
    {
        //make sure bitOffset is always incremented, so that next item starts at correct offset
        *bitOffset = (*bitOffset + size()) % 8;
        return false;
    }
    AllPrimitiveTypes oldVal(value());
    // this is implemented in the subclasses
    AllPrimitiveTypes valToWrite = qVariantToAllPrimitiveTypes(valueVariant);
    AllPrimitiveTypes newVal(oldVal);
    //this handles remaining < size() for us
    mWasAbleToRead = newVal.writeBits(size(), valToWrite, out, byteOrder(), address, bitsRemaining, bitOffset);

    //no need for dataChanged, since structtool ensures that the model emits dataChanged when items are set
    return true;
}

Qt::ItemFlags PrimitiveDataInformation::flags(int column, bool fileLoaded) const
{
    if (column == 2 && fileLoaded)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

qint64 PrimitiveDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset)
{
    if (bitsRemaining < (unsigned)size())
    {
        mWasAbleToRead = false;
        setValue(0);
        return -1;
    }
    bool wasValid = mWasAbleToRead;
    AllPrimitiveTypes oldVal(value());
    AllPrimitiveTypes newVal(value());

    mWasAbleToRead = newVal.readBits(size(), input, byteOrder(), address, bitsRemaining, bitOffset);

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
    QScriptValue ret = engine->newObject(handlerInfo->mPrimitiveClass);
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012  Alex Richardson <alex.richardson@gmx.de>
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
#include "basicprimitivedatainformation.h"

#include "booldatainformation.h"
#include "chardatainformation.h"
#include "doubledatainformation.h"
#include "floatdatainformation.h"
#include "sintdatainformation.h"
#include "uintdatainformation.h"
#include "../topleveldatainformation.h"
#include "../../script/scripthandlerinfo.h"
#include "../../script/scriptlogger.h"
#include "../../script/classes/primitivescriptclass.h"

#include <QScriptValue>
#include <QScriptEngine>

#include <abstractbytearraymodel.h>

template<typename T, typename C>
inline QScriptValue BasicPrimitiveDataInformation<T, C>::toScriptValue(QScriptEngine* engine,
        ScriptHandlerInfo* handlerInfo)
{
    QScriptValue ret = engine->newObject(handlerInfo->mPrimitiveClass.data());
    ret.setData(engine->toScriptValue(static_cast<DataInformation*>(this)));
    return ret;
}

template<typename T, typename C>
QString BasicPrimitiveDataInformation<T, C>::valueString() const
{
    if (!mWasAbleToRead)
        return i18nc("invalid value (out of range)", "&lt;invalid&gt;");
    return C::staticValueString(mValue);
}

template<typename T, typename C>
QScriptValue BasicPrimitiveDataInformation<T, C>::valueAsQScriptValue() const
{
    return C::asScriptValue(mValue, 0, 0);
}

template<typename T, typename C>
bool BasicPrimitiveDataInformation<T, C>::setData(const QVariant& value,
        Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining,
        quint8 bitOffset)
{
    T oldVal(this->mValue);
    bool ok;
    T valToWrite = C::fromVariant(value, &ok);
    if (!ok)
    {
        logError() << "Failed to convert" << value << "to" << C::staticType();
        return false;
    }
    AllPrimitiveTypes newVal(oldVal);
    //this handles remaining < size() for us
    bool wasAbleToWrite = newVal.writeBits(sizeof(T) * 8, valToWrite, out, effectiveByteOrder(), address,
            bitsRemaining, &bitOffset);
    return wasAbleToWrite;
}

template<typename T, typename C>
qint64 BasicPrimitiveDataInformation<T, C>::readData(Okteta::AbstractByteArrayModel* input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(BitCount64(input->size() - address) * 8 - *bitOffset == bitsRemaining);
    const bool wasValid = mWasAbleToRead;
    if (bitsRemaining < BitCount64(size()))
    {
        mWasAbleToRead = false;
        mValue = 0;

        if (wasValid != mWasAbleToRead)
            topLevelDataInformation()->setChildDataChanged();
        return -1;
    }
    else
    {
        mWasAbleToRead = true;
        const T oldVal = this->mValue;
        //bit offset will always stay the same since type T uses a full number of bytes
        mValue = AllPrimitiveTypes::readValue<T>(input, address, effectiveByteOrder(), *bitOffset);

        if (oldVal != mValue || wasValid != mWasAbleToRead)
            topLevelDataInformation()->setChildDataChanged();
        return size();
    }
}

//specify all the specializations so we can move code to the cpp file
template class BasicPrimitiveDataInformation<quint8, UIntDataInformation<quint8> > ;
template class BasicPrimitiveDataInformation<quint16, UIntDataInformation<quint16> > ;
template class BasicPrimitiveDataInformation<quint32, UIntDataInformation<quint32> > ;
template class BasicPrimitiveDataInformation<quint64, UIntDataInformation<quint64> > ;
template class BasicPrimitiveDataInformation<quint8, BoolDataInformation<quint8> > ;
template class BasicPrimitiveDataInformation<quint16, BoolDataInformation<quint16> > ;
template class BasicPrimitiveDataInformation<quint32, BoolDataInformation<quint32> > ;
template class BasicPrimitiveDataInformation<quint64, BoolDataInformation<quint64> > ;
template class BasicPrimitiveDataInformation<qint8, SIntDataInformation<qint8> > ;
template class BasicPrimitiveDataInformation<qint16, SIntDataInformation<qint16> > ;
template class BasicPrimitiveDataInformation<qint32, SIntDataInformation<qint32> > ;
template class BasicPrimitiveDataInformation<qint64, SIntDataInformation<qint64> > ;
template class BasicPrimitiveDataInformation<quint8, CharDataInformation> ;
template class BasicPrimitiveDataInformation<float, FloatDataInformation> ;
template class BasicPrimitiveDataInformation<double, DoubleDataInformation> ;

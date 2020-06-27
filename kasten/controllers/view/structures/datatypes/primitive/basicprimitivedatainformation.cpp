/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "basicprimitivedatainformation.hpp"

#include "booldatainformation.hpp"
#include "chardatainformation.hpp"
#include "doubledatainformation.hpp"
#include "floatdatainformation.hpp"
#include "sintdatainformation.hpp"
#include "uintdatainformation.hpp"
#include "../topleveldatainformation.hpp"
#include "../../script/scripthandlerinfo.hpp"
#include "../../script/scriptlogger.hpp"
#include "../../script/classes/primitivescriptclass.hpp"

#include <QScriptValue>
#include <QScriptEngine>

#include <KLocalizedString>

#include <Okteta/AbstractByteArrayModel>

template <typename T, class C>
inline QScriptClass* BasicPrimitiveDataInformation<T, C>::scriptClass(ScriptHandlerInfo* handlerInfo) const
{
    return handlerInfo->mPrimitiveClass.data();
}

template <typename T, typename C>
QScriptValue BasicPrimitiveDataInformation<T, C>::valueAsQScriptValue() const
{
    return C::asScriptValue(mValue, nullptr, nullptr);
}

template <typename T, typename C>
bool BasicPrimitiveDataInformation<T, C>::setData(const QVariant& value,
                                                  Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining,
                                                  quint8 bitOffset)
{
    T oldVal(this->mValue);
    bool ok;
    T valToWrite = C::fromVariant(value, &ok);
    if (!ok) {
        logError() << "Failed to convert" << value << "to" << C::staticType();
        return false;
    }
    AllPrimitiveTypes newVal(oldVal);
    // this handles remaining < size() for us
    bool wasAbleToWrite = newVal.writeBits(sizeof(T) * 8, valToWrite, out, effectiveByteOrder(), address,
                                           bitsRemaining, &bitOffset);
    return wasAbleToWrite;
}

template <typename T, typename C>
qint64 BasicPrimitiveDataInformation<T, C>::readData(Okteta::AbstractByteArrayModel* input,
                                                     Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading except
    const bool wasValid = mWasAbleToRead;
    if (bitsRemaining < BitCount64(size())) {
        mWasAbleToRead = false;
        mValue = 0;

        if (wasValid != mWasAbleToRead) {
            topLevelDataInformation()->setChildDataChanged();
        }
        return -1;
    }

    Q_ASSERT(BitCount64(input->size() - address) * 8 - *bitOffset >= bitsRemaining);
    mWasAbleToRead = true;
    const T oldVal = this->mValue;
    // bit offset will always stay the same since type T uses a full number of bytes
    mValue = AllPrimitiveTypes::readValue<T>(input, address, effectiveByteOrder(), *bitOffset);

    if (oldVal != mValue || wasValid != mWasAbleToRead) {
        topLevelDataInformation()->setChildDataChanged();
    }
    return size();
}

// specify all the specializations so we can move code to the cpp file
template class BasicPrimitiveDataInformation<quint8, UIntDataInformationMethods<quint8>>;
template class BasicPrimitiveDataInformation<quint16, UIntDataInformationMethods<quint16>>;
template class BasicPrimitiveDataInformation<quint32, UIntDataInformationMethods<quint32>>;
template class BasicPrimitiveDataInformation<quint64, UIntDataInformationMethods<quint64>>;
template class BasicPrimitiveDataInformation<quint8, BoolDataInformationMethods<quint8>>;
template class BasicPrimitiveDataInformation<quint16, BoolDataInformationMethods<quint16>>;
template class BasicPrimitiveDataInformation<quint32, BoolDataInformationMethods<quint32>>;
template class BasicPrimitiveDataInformation<quint64, BoolDataInformationMethods<quint64>>;
template class BasicPrimitiveDataInformation<qint8, SIntDataInformationMethods<qint8>>;
template class BasicPrimitiveDataInformation<qint16, SIntDataInformationMethods<qint16>>;
template class BasicPrimitiveDataInformation<qint32, SIntDataInformationMethods<qint32>>;
template class BasicPrimitiveDataInformation<qint64, SIntDataInformationMethods<qint64>>;
template class BasicPrimitiveDataInformation<quint8, CharDataInformationMethods>;
template class BasicPrimitiveDataInformation<float, FloatDataInformationMethods>;
template class BasicPrimitiveDataInformation<double, DoubleDataInformationMethods>;

/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
 *
 *   Copyright 2010 Alex Richardson <alex.richardson@gmx.de>
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

#include "additionaldata.h"

AdditionalData::AdditionalData(QScriptValue* validate, QScriptValue* update) :
    mUpdateFunction(update), mValidationFunction(validate), mValidationError(
            QString())
{
}

AdditionalData::AdditionalData(const AdditionalData& data) :
    mUpdateFunction(0), mValidationFunction(0)
{
    if (data.mUpdateFunction)
        this->mUpdateFunction = new QScriptValue(*(data.mUpdateFunction));
    if (data.mValidationFunction)
        this->mValidationFunction = new QScriptValue(*(data.mValidationFunction));
    if (!data.mValidationError.isEmpty())
        mValidationError = data.mValidationError;
}

AdditionalData::~AdditionalData()
{
    delete mValidationFunction;
    delete mUpdateFunction;
}

QScriptValue* AdditionalData::updateFunction() const
{
    return mUpdateFunction;
}
void AdditionalData::setUpdateFunction(QScriptValue* func)
{
    mUpdateFunction = func;
}

QScriptValue* AdditionalData::validationFunction() const
{
    return mValidationFunction;
}
void AdditionalData::setValidationFunction(QScriptValue* func)
{
    mValidationFunction = func;
}

QString AdditionalData::validationError() const
{
    return mValidationError;
}
void AdditionalData::setValidationError(QString error)
{
    mValidationError = error;
}

/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2010, 2011 Alex Richardson <alex.richardson@gmx.de>
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

#ifndef ADDITIONALDATA_H_
#define ADDITIONALDATA_H_

#include <QScriptValue>
#include <QSharedData>

/** Additional data which is held as a pointer by the *DataInformation classes
 *  to reduce memory consumption, since this will not always be needed */
class AdditionalData
{
public:
    explicit AdditionalData(const QScriptValue& validate = QScriptValue(),
            const QScriptValue& update = QScriptValue());
    AdditionalData(const AdditionalData& data);
    ~AdditionalData();

    const QScriptValue& updateFunction() const;
    void setUpdateFunction(const QScriptValue& func);
    const QScriptValue& validationFunction() const;
    void setValidationFunction(const QScriptValue& func);
    QString validationError() const;
    void setValidationError(const QString& error);
    private:
    /** the function called when new data is read, to update the structure */
    QScriptValue mUpdateFunction;
    /** this function is called after all data has been read to validate the structure */
    QScriptValue mValidationFunction;
    QString mValidationError;
};

inline const QScriptValue& AdditionalData::updateFunction() const
{
    return mUpdateFunction;
}

inline void AdditionalData::setUpdateFunction(const QScriptValue& func)
{
    mUpdateFunction = func;
}

inline const QScriptValue& AdditionalData::validationFunction() const
{
    return mValidationFunction;
}

inline void AdditionalData::setValidationFunction(const QScriptValue& func)
{
    mValidationFunction = func;
}

inline QString AdditionalData::validationError() const
{
    return mValidationError;
}

inline void AdditionalData::setValidationError(const QString& error)
{
    mValidationError = error;
}

inline AdditionalData::AdditionalData(const QScriptValue& validate, const QScriptValue& update)
        : mUpdateFunction(update), mValidationFunction(validate), mValidationError(QString())
{
}

inline AdditionalData::AdditionalData(const AdditionalData& data)
        : mUpdateFunction(data.mUpdateFunction), mValidationFunction(
                data.mValidationFunction), mValidationError(data.mValidationError)
{
}

inline AdditionalData::~AdditionalData()
{
}

#endif /* ADDITIONALDATA_H_ */

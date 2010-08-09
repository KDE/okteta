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

#ifndef ADDITIONALDATA_H_
#define ADDITIONALDATA_H_

#include <QtScript/QScriptValue>

/** Additional data which is held as a pointer by the *DataInformation classes
 *  to reduce memory consumption, since this will not always be needed */
class AdditionalData
{
public:
    explicit AdditionalData(QScriptValue* validate = NULL, QScriptValue* update = NULL);
    AdditionalData(const AdditionalData& data);
    virtual ~AdditionalData();
private:
    /** the function called when new data is read, to update the structure */
    QScriptValue* mUpdateFunction;
    /** this function is called after all data has been read to validate the structure */
    QScriptValue* mValidationFunction;
    QString mValidationError;
public:
    QScriptValue* updateFunction() const;
    void setUpdateFunction(QScriptValue* func);
    QScriptValue* validationFunction() const;
    void setValidationFunction(QScriptValue* func);
    QString validationError() const;
    void setValidationError(QString error);
};

#endif /* ADDITIONALDATA_H_ */

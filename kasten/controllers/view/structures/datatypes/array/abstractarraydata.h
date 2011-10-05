/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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


#ifndef ABSTRACTARRAYDATA_H
#define ABSTRACTARRAYDATA_H

#include <address.h>
#include "../datainformationbase.h"
#include "../../allprimitivetypes.h"

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtScript/QScriptValue>


class QWidget;
class DataInformation;
class ScriptHandlerInfo;
namespace Okteta {
    class AbstractByteArrayModel;
}

class AbstractArrayData
{
    Q_DISABLE_COPY(AbstractArrayData)
public:
    explicit AbstractArrayData(DataInformation* parent);
    virtual ~AbstractArrayData();

    virtual QVariant dataAt(int index, int column, int role) = 0;

    virtual unsigned int length() const = 0;
    virtual void setLength(int newLength) = 0;

    virtual QString typeName() const = 0;

    virtual BitCount32 size() const = 0;

    virtual DataInformation* childAt(unsigned int idx) = 0;

    virtual QScriptValue toScriptValue(uint index, QScriptEngine* engine,
            ScriptHandlerInfo* handlerInfo) const = 0;
    /** the primitive type or Type_NotPrimitive for structs etc */
    virtual PrimitiveDataType primitiveType() const = 0;
    virtual void setParent(DataInformation* parent) = 0;

    virtual int indexOf(const DataInformation* data) const = 0;
    virtual BitCount32 offset(uint row) const = 0;
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) = 0;
    virtual bool setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining) = 0;
    virtual BitCount32 sizeAt(int index) = 0;
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) = 0;
    virtual bool isComplex() const = 0;

    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const = 0;
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const = 0;
    virtual void setChildWidgetData(uint index, QWidget* w) const = 0;
protected:
    DataInformation* mParent;
};


#endif // ABSTRACTARRAYDATA_H

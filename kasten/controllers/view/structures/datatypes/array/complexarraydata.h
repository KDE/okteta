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



#ifndef COMPLEXARRAYDATA_H
#define COMPLEXARRAYDATA_H

#include "abstractarraydata.h"

#include <QtCore/QVector>

class DataInformation;
class ScriptHandlerInfo;

class ComplexArrayData : public AbstractArrayData
{
    Q_DISABLE_COPY(ComplexArrayData)
public:
    /** Takes ownership of @p data !*/
    ComplexArrayData(unsigned int initialLength, DataInformation* data, DataInformation* parent);
    virtual ~ComplexArrayData();

    virtual void setLength(int newLength);

    virtual QVariant dataAt(int index, int column, int role);

    virtual unsigned int length() const;
    virtual BitCount32 size() const;

    virtual QString typeName() const;

    virtual DataInformation* childAt(unsigned int idx);
    virtual int indexOf(const DataInformation* data) const;
    virtual BitCount32 offset(uint row) const;
    virtual BitCount32 sizeAt(int index);
    virtual Qt::ItemFlags childFlags(int row, int column, bool fileLoaded);

    virtual PrimitiveDataType primitiveType() const;
    virtual bool isComplex() const;

    QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) const;

    /** have to override this, to set correct parent of mChildType -> crash otherwise */
    virtual void setParent(DataInformation* parent);

    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining);
    virtual bool setChildData(uint row, QVariant value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining);
    /** returns a COPY of the currently stored child type */
    DataInformation* childType() const;

    virtual QWidget* createChildEditWidget(uint index, QWidget* parent) const;
    virtual QVariant dataFromChildWidget(uint index, const QWidget* w) const;
    virtual void setChildWidgetData(uint index, QWidget* w) const;
private:
    DataInformation* mChildType;
    QVector<DataInformation*> mChildren;
};

inline bool ComplexArrayData::isComplex() const
{
    return true;
}

#endif // COMPLEXARRAYDATA_H

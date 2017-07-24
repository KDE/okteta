/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include <QVector>

class DataInformation;
class ScriptHandlerInfo;

class ComplexArrayData : public AbstractArrayData
{
    Q_DISABLE_COPY(ComplexArrayData)
public:
    /** Takes ownership of @p data !*/
    ComplexArrayData(unsigned int initialLength, DataInformation* data, ArrayDataInformation* parent);
    ~ComplexArrayData() override;


    QVariant dataAt(uint index, int column, int role) override;

    unsigned int length() const override;
    void setLength(uint newLength) override;
    BitCount32 size() const override;

    QString typeName() const override;

    DataInformation* childAt(unsigned int idx) override;
    int indexOf(const DataInformation* data) const override;
    BitCount64 offset(const DataInformation* child) const override;
    BitCount32 sizeAt(uint index) override;
    Qt::ItemFlags childFlags(int row, int column, bool fileLoaded) override;

    PrimitiveDataType primitiveType() const override;
    bool isComplex() const override;

    QScriptValue toScriptValue(uint index, QScriptEngine* engine, ScriptHandlerInfo* handlerInfo) override;

    qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) override;
    bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
                      Okteta::Address address, BitCount64 bitsRemaining) override;

    QWidget* createChildEditWidget(uint index, QWidget* parent) const override;
    QVariant dataFromChildWidget(uint index, const QWidget* w) const override;
    void setChildWidgetData(uint index, QWidget* w) const override;
protected:
    void setNewParentForChildren() override;
private:
    void appendChildren(uint from, uint to);
private:
    QVector<DataInformation*> mChildren;
};

inline bool ComplexArrayData::isComplex() const
{
    return true;
}

#endif // COMPLEXARRAYDATA_H

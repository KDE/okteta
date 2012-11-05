/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alessandro Di Federico <ale@clearmind.me>
 *   Copyright 2012 Alex Richardson <alex.richardosn@gmx.de>
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

#ifndef POINTERDATAINFORMATION_H
#define POINTERDATAINFORMATION_H

#include "primitivedatainformation.h"


class PointerDataInformation : public PrimitiveDataInformation
{
    DATAINFORMATION_CLONE_DECL(PointerDataInformation, PrimitiveDataInformation);
public:
    /** creates a new pointer
     *  takes ownership over @p childType and @p valueType
     */
    PointerDataInformation(QString name, DataInformation* childType,
            PrimitiveDataInformation* valueType, DataInformation* parent);
    virtual ~PointerDataInformation();
protected:
    QScopedPointer<PrimitiveDataInformation> mValue; // To allow different pointer sizes
    QScopedPointer<DataInformation> mPointerTarget;

public:
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out, Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset);
    virtual BitCount32 size() const;
    virtual void setWidgetData(QWidget* w) const;
    virtual QVariant dataFromWidget(const QWidget* w) const;
    virtual QWidget* createEditWidget(QWidget* parent) const;
    virtual QString typeName() const;
    virtual QString valueString() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    virtual bool canHaveChildren() const;
    virtual uint childCount() const;
    virtual DataInformation* childAt(uint index) const;
    virtual PrimitiveDataType type() const;
    virtual AllPrimitiveTypes value() const;
    virtual void setValue(AllPrimitiveTypes newValue);
    virtual QScriptValue valueAsQScriptValue() const;
    virtual bool isPointer() const;
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const;
    virtual int indexOf(const DataInformation* const data) const;

    void delayedReadData(Okteta::AbstractByteArrayModel* input, Okteta::Address address);

    DataInformation* pointerTarget() const;
    /** Set a new pointer target
     * @param target the new target (ownership is taken)
     */
    void setPointerTarget(DataInformation* target);

    PrimitiveDataInformation* pointerType() const;
    /** Set a new pointer target
     * @param type the new pointer type (ownership is taken if return value is true)
     * @return true if type was set, false if not
     */
    bool setPointerType(DataInformation* type);
};

inline bool PointerDataInformation::canHaveChildren() const
{
    return true;
}

inline PrimitiveDataType PointerDataInformation::type() const
{
    return Type_Invalid; //FIXME add special type (pointer + bitfield)
}

inline bool PointerDataInformation::isPointer() const
{
    return true;
}

inline DataInformation* PointerDataInformation::pointerTarget() const
{
    return mPointerTarget.data();
}

inline void PointerDataInformation::setPointerTarget(DataInformation* target)
{
    Q_CHECK_PTR(target);
    mPointerTarget.reset(target);
    mPointerTarget->setParent(this);
}

inline PrimitiveDataInformation* PointerDataInformation::pointerType() const
{
    return mValue.data();
}

#endif // POINTERDATAINFORMATION_H

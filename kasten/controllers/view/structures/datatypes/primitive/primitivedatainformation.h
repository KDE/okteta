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
#ifndef PRIMITIVEDATAINFORMATION_H_
#define PRIMITIVEDATAINFORMATION_H_

#include "../datainformation.h"
#include "../../allprimitivetypes.h"
#include <QtScript/QScriptValue>

class PrimitiveDataInformation: public DataInformation
{
public:
    explicit PrimitiveDataInformation(QString name, DataInformation* parent = NULL);
    virtual ~PrimitiveDataInformation();
    virtual PrimitiveDataInformation* clone() const = 0;
    virtual BitCount32 size() const = 0;
    virtual QString typeName() const = 0;
    static QString typeName(PrimitiveDataType type);

    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;

    virtual bool setData(const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual bool setChildData(uint row, const QVariant& value, Okteta::AbstractByteArrayModel* out,
            Okteta::Address address, BitCount64 bitsRemaining, quint8 bitOffset);
    virtual qint64 readData(Okteta::AbstractByteArrayModel *input,
            Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset);

    virtual QString valueString() const = 0;
    virtual PrimitiveDataType type() const = 0;
    virtual AllPrimitiveTypes value() const = 0;
    virtual void setValue(AllPrimitiveTypes newVal) = 0;
    virtual BitCount32 childSize(int index) const;
    virtual AllPrimitiveTypes qVariantToAllPrimitiveTypes(const QVariant& value) const = 0;
    virtual QScriptValue valueAsQScriptValue() const = 0;
    virtual QScriptValue toScriptValue(QScriptEngine* engine, ScriptHandlerInfo* handlerInfo);

    virtual bool isPrimitive() const;
protected:
    virtual BitCount32 offset(unsigned int index) const;
    explicit PrimitiveDataInformation(const PrimitiveDataInformation& d);
};

inline BitCount32 PrimitiveDataInformation::childSize(int) const
{
    Q_ASSERT_X(false, "PrimitiveDataInformation::childSize", "This should never be called");
    return 0;
}


inline BitCount32 PrimitiveDataInformation::offset(unsigned int index) const
{
    Q_UNUSED(index)
    Q_ASSERT_X(false, "PrimitiveDataInformation::offset", "This should never be called");
    return 0;
}

inline bool PrimitiveDataInformation::isPrimitive() const
{
    return true;
}

#define PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTRUCTORS(type,superType) public: \
        type##DataInformation(QString name, DataInformation* parent = NULL) :\
                superType##DataInformation(name, parent), mValue(0) {}\
        virtual ~type##DataInformation() {} \
    protected: \
        type##DataInformation(const type##DataInformation& d) : \
        superType##DataInformation(d), mValue(d.mValue) {} \
    private:

#endif /* PRIMITIVEDATAINFORMATION_H_ */

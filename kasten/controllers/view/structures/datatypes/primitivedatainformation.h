/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#include "datainformation.h"
#include "../allprimitivetypes.h"

class PrimitiveDataInformation: public DataInformation
{
Q_OBJECT
public:
Q_PROPERTY(bool valid READ isValid())

protected:
    AllPrimitiveTypes mValue; //TODO abstract into subclasses -> more flexibility
    PrimitiveDataType mType; //TODO unnecessary?
    bool mIsValid :1;
    /** get the appropriate base for QString::number from StructDisplaySettings */
    explicit PrimitiveDataInformation(const PrimitiveDataInformation& d);
public:
    explicit PrimitiveDataInformation(QString name, PrimitiveDataType type, int index = -1,
            DataInformation* parent = NULL);
    virtual ~PrimitiveDataInformation();
    static PrimitiveDataInformation* newInstance(QString name,
            PrimitiveDataType type, int index = -1, DataInformation* parent = NULL);
    //    static int getStringConversionBase(PrimitiveDataType type);
    //    static QString getTypeName(PrimitiveDataType type);
    //    static int getSize(PrimitiveDataType type);
    static QString getValueString(AllPrimitiveTypes value, PrimitiveDataType type,
            int base);
    virtual DataInformation* clone() const = 0;
    virtual PrimitiveDataType type() const
    {
        return mType;
    }
    virtual int getSize() const = 0;
    virtual inline QString getTypeName() const = 0;
    virtual inline bool isValid() const
    {
        return mIsValid;
    }
    virtual int displayBase() const = 0;
    QVariant primitiveValue() const;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded = true) const;
    /** @return true if data was changed */
    virtual bool setData(const QVariant &value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining);
    virtual QString getValueString() const = 0;
    AllPrimitiveTypes value() const
    {
        return mValue;
    }
protected:
    void setModelData(AllPrimitiveTypes value, Okteta::AbstractByteArrayModel *out,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining);
    virtual Okteta::Size offset(unsigned int index) const
    {
        Q_UNUSED(index)
        return 0;
    }
public:
    QVariant data(int column, int role) const;
    virtual Okteta::Size readData(Okteta::AbstractByteArrayModel* input,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining);
    static PrimitiveDataType typeStringToType(QString& typeStr);

};
#define PRIMITIVEDATAINFORMATION_SUBCLASS_CONSTURUCTORS(type,superType) public: \
        type##DataInformation(QString name, PrimitiveDataType dataType,\
                int index = -1, DataInformation* parent = NULL) :\
                superType##DataInformation(name, dataType, index, parent) {}\
        virtual ~type##DataInformation() {} \
    protected: \
        type##DataInformation(const type##DataInformation& d) : \
        superType##DataInformation(d) {} \
    private:

#endif /* PRIMITIVEDATAINFORMATION_H_ */

/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
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
#ifndef ABSTRACTBITFIELDDATAINFORMATION_H_
#define ABSTRACTBITFIELDDATAINFORMATION_H_

#include "primitivedatainformation.h"

class AbstractBitfieldDataInformation: public PrimitiveDataInformation
{
Q_OBJECT
public:
    AbstractBitfieldDataInformation(QString name, PrimitiveDataType dataType,
            uint width, int index = -1, DataInformation* parent = 0) :
        PrimitiveDataInformation(name, dataType, index, parent), mWidth(width)
    {
    }
    virtual ~AbstractBitfieldDataInformation()
    {
    }
protected:
    AbstractBitfieldDataInformation(const AbstractBitfieldDataInformation& d) :
        PrimitiveDataInformation(d), mWidth(d.mWidth)
    {
    }
private:
    unsigned mWidth :7; //cannot be more than 64 since a quint64 is used for storage
public:
    inline uint width() const
    {
        return mWidth;
    }
    virtual QString sizeString() const;

    inline int size() const
    {
        return width();
    }
    inline quint64 mask() const
    {
        return (1 << width()) - 1;
        /* same as:
         *
         * quint64 ret = 0;
         * for (int i = 0; i < width(); i++)
         *     ret |= 1 << i;
         * return ret;
         */
    }
    virtual QString typeName() const
    {
        return i18ncp("Data type", "bitfield (%1 bit wide)",
                "bitfield (%1 bits wide)", width());
    }
    virtual bool setData(const QVariant &value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining, quint8* bitOffset);
    virtual Okteta::Size readData(Okteta::AbstractByteArrayModel* input,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining,
            quint8* bitOffset);
    virtual QWidget* createEditWidget(QWidget* parent) const = 0;
    virtual QVariant dataFromWidget(const QWidget* w) const = 0;
    virtual void setWidgetData(QWidget* w) const = 0;
    virtual Qt::ItemFlags flags(int column, bool fileLoaded) const
    {
        if (column == 2 && fileLoaded)
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled /*| Qt::ItemIsEditable*/;
            //FIXME uncomment once editing works
        else
            return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
private:
    AllPrimitiveTypes readDataLittleEndian(Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint8* bitOffset);
    void setDataLittleEndian(const QVariant &value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address,
            quint8* bitOffset);
    AllPrimitiveTypes readDataBigEndian(Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint8* bitOffset);
    void setDataBigEndian(const QVariant &value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address,
            quint8* bitOffset);

};
#endif /* ABSTRACTBITFIELDDATAINFORMATION_H_ */

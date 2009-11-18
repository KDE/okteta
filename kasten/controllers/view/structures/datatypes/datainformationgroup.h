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
#ifndef DATAINFORMATIONGROUP_H_
#define DATAINFORMATIONGROUP_H_

#include "datainformationwithchildren.h"

class DataInformationGroup: public DataInformationWithChildren
{
Q_OBJECT
    DataInformationGroup(const DataInformationGroup& d);
public:
    DataInformationGroup(QString name, DataInformation* parent);
    //TODO add nested groups
    virtual ~DataInformationGroup();
    virtual inline DataInformation* clone() const
    {
        return new DataInformationGroup(*this);
    }
    virtual QVariant data(int column, int role) const;
    virtual inline int getSize() const
    {
        return 0; //return zero as this is not needed
    }
    QString getTypeName() const;

    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *input, ByteOrder byteOrder,
            Okteta::Address address, Okteta::Size remaining);
    virtual Okteta::Size readData(Okteta::AbstractByteArrayModel* input,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining);
    inline DataInformationGroup& operator<<(DataInformation* child)
    {
        appendChild(child);
        return *this;
    }
protected:
    virtual Okteta::Size offset(unsigned int index) const
    {
        Q_UNUSED(index)
        return 0;
    }
};

#endif /* DATAINFORMATIONGROUP_H_ */

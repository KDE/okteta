/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef UNIONDATAINFORMATION_H_
#define UNIONDATAINFORMATION_H_

#include "datainformationwithchildren.h"

/** A class holding the data of a union for Okteta*/
class UnionDataInformation : public DataInformationWithChildren
{
    DATAINFORMATION_CLONE(UnionDataInformation, DataInformationWithChildren) {}
public:
    explicit UnionDataInformation(const QString& name, const QVector<DataInformation*>& children
            = QVector<DataInformation*>(), DataInformation* parent = 0);
    virtual ~UnionDataInformation();

    virtual bool isUnion() const;
    //implement the DataInformation pure virtual functions
    virtual QString typeName() const;
    virtual BitCount32 size() const;
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const;
};

inline bool UnionDataInformation::isUnion() const
{
    return true;
}

#endif /* UNIONDATAINFORMATION_H_ */

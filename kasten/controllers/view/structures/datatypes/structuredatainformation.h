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
#ifndef STRUCTUREDATAINFORMATION_H_
#define STRUCTUREDATAINFORMATION_H_

#include "datainformationwithchildren.h"

/** A class holding the data of a struct for Okteta*/
class StructureDataInformation : public DataInformationWithChildren
{
    DATAINFORMATION_CLONE(StructureDataInformation, DataInformationWithChildren) {}
public:
    explicit StructureDataInformation(const QString& name, const QVector<DataInformation*>& children =
            QVector<DataInformation*>(), DataInformation* parent = 0);
    virtual ~StructureDataInformation();

    virtual qint64 readData(Okteta::AbstractByteArrayModel *input, Okteta::Address address,
            BitCount64 bitsRemaining, quint8* bitOffset);
    virtual bool isStruct() const;

    QString typeName() const;
    virtual BitCount64 childPosition(const DataInformation* child, Okteta::Address start) const;

    static bool readChildren(const QVector<DataInformation*> children, Okteta::AbstractByteArrayModel *input,
            Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset, qint64* readBitsPtr,
            TopLevelDataInformation* top);
};

inline bool StructureDataInformation::isStruct() const
{
    return true;
}

#endif /* STRUCTUREDATAINFORMATION_H_ */

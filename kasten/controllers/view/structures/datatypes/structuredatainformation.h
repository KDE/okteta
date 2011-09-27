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
class StructureDataInformation: public DataInformationWithChildren
{
protected:
    explicit StructureDataInformation(const StructureDataInformation& d);
public:
    explicit StructureDataInformation(QString name, DataInformation* parent = NULL);
    virtual ~StructureDataInformation();
    DATAINFORMATION_CLONE(Structure)
public:
    void addDataTypeToStruct(DataInformation* dataInformation);
public:
    virtual bool isStruct() const;
    //implement the DataInformation pure virtual functions
    QString typeName() const;
};

inline bool StructureDataInformation::isStruct() const
{
    return true;
}

#endif /* STRUCTUREDATAINFORMATION_H_ */

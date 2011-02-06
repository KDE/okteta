/*
 *   This file is part of the Okteta Kasten Framework, part of the KDE project.
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
class UnionDataInformation: public DataInformationWithChildren
{
Q_OBJECT
protected:
    explicit UnionDataInformation(const UnionDataInformation& d);
public:
    explicit UnionDataInformation(QString name, DataInformation* parent = NULL);
    virtual ~UnionDataInformation();
    DATAINFORMATION_CLONE(Union)

public Q_SLOTS:
    void addDataTypeToUnion(DataInformation* dataInformation);
public:
    //implement the DataInformation pure virtual functions
    QString typeName() const;
    virtual int size() const;
    virtual qint64 readData(Okteta::AbstractByteArrayModel* input,
            Okteta::Address address, quint64 bitsRemaining, quint8* bitOffset);
    virtual bool setData(const QVariant& value, DataInformation* inf,
            Okteta::AbstractByteArrayModel *out, Okteta::Address address,
            quint64 bitsRemaining, quint8* bitOffset);
    /** add another field to this union */
    UnionDataInformation& operator<<(DataInformation* field);
protected:
    quint64 offset(unsigned int index) const;
};


inline quint64 UnionDataInformation::offset(unsigned int index) const
{
    Q_UNUSED(index)
    return 0;
}

#endif /* UNIONDATAINFORMATION_H_ */

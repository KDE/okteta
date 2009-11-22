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

#ifndef DYNAMICLENGHTARRAYDATAINFORMATION_H_
#define DYNAMICLENGHTARRAYDATAINFORMATION_H_

#include "abstractarraydatainformation.h"

//TODO merge dynamic and static length
class DynamicLengthArrayDataInformation: public AbstractArrayDataInformation
{
Q_OBJECT
protected:
    DynamicLengthArrayDataInformation(const DynamicLengthArrayDataInformation& d);
public:
    /** creates a new array with static length.
     *  children is used as the base type of the array and is cloned length times.
     *
     *  length should be > 0
     */
    DynamicLengthArrayDataInformation(QString name, const QString& lengthStr,
            const DataInformation& childItem, int index = -1,
            DataInformation* parent = NULL);
    virtual ~DynamicLengthArrayDataInformation();
    DATAINFORMATION_CLONE(DynamicLengthArray)

    virtual Okteta::Size readData(Okteta::AbstractByteArrayModel* input,
            ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining);
private:
    int calculateLength();
    void resizeChildren();
    QString mLengthString;
    DataInformation* mChildType;
};
#endif /* DYNAMICLENGHTARRAYDATAINFORMATION_H_ */

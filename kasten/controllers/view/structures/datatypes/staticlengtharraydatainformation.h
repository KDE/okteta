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
#ifndef STATICLENGTHARRAYDATAINFORMATION_H_
#define STATICLENGTHARRAYDATAINFORMATION_H_
#include "abstractarraydatainformation.h"

class StaticLengthArrayDataInformation: public AbstractArrayDataInformation
{
Q_OBJECT
protected:
    StaticLengthArrayDataInformation(const StaticLengthArrayDataInformation& d);
public:
    /** creates a new array with static length.
     *  children is used as the base type of the array and is cloned length times.
     *
     *  length should be > 0
     */
    StaticLengthArrayDataInformation(QString name, unsigned int length,
            const DataInformation& childItem, int index = -1,
            DataInformation* parent = NULL);
    virtual ~StaticLengthArrayDataInformation();
    DATAINFORMATION_CLONE(StaticLengthArray)
};

#endif /* STATICLENGTHARRAYDATAINFORMATION_H_ */

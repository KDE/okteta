/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
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
#include "datainformationbase.h"
#include "topleveldatainformation.h"

DataInformationBase::DataInformationBase()
{

}

DataInformationBase::~DataInformationBase()
{

}

bool DataInformationBase::isArray() const
{
    return false;
}

bool DataInformationBase::isDummy() const
{
    return false;
}

bool DataInformationBase::isPrimitive() const
{
    return false;
}

bool DataInformationBase::isEnum() const
{
    return false;
}

bool DataInformationBase::isStruct() const
{
    return false;
}

bool DataInformationBase::isUnion() const
{
    return false;
}

bool DataInformationBase::isBitfield() const
{
    return false;
}

bool DataInformationBase::isString() const
{
    return false;
}

TopLevelDataInformation* DataInformationBase::asTopLevel()
{
    Q_ASSERT(isTopLevel());
    //we need dynamic cast due to multiple inheritance, even it this class has no members
    return dynamic_cast<TopLevelDataInformation*>(this);
}

const TopLevelDataInformation* DataInformationBase::asTopLevel() const
{
    Q_ASSERT(isTopLevel());
    //we need dynamic cast due to multiple inheritance, even it this class has no members
    return dynamic_cast<const TopLevelDataInformation*>(this);
}

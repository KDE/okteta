/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "datainformationbase.hpp"
#include "topleveldatainformation.hpp"

DataInformationBase::DataInformationBase() = default;

DataInformationBase::~DataInformationBase() = default;

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

bool DataInformationBase::isPointer() const
{
    return false;
}

bool DataInformationBase::isTaggedUnion() const
{
    return false;
}

bool DataInformationBase::isDataInformationWithChildren() const
{
    return false;
}

bool DataInformationBase::isDataInformationWithDummyChildren() const
{
    return false;
}

TopLevelDataInformation* DataInformationBase::asTopLevel()
{
    // multiple inheritance -> we can't use reinterpret_cast -> we have to include the file
    return isTopLevel() ? static_cast<TopLevelDataInformation*>(this) : nullptr;
}

const TopLevelDataInformation* DataInformationBase::asTopLevel() const
{
    // multiple inheritance -> we can't use reinterpret_cast -> we have to include the file
    return isTopLevel() ? static_cast<const TopLevelDataInformation*>(this) : nullptr;
}

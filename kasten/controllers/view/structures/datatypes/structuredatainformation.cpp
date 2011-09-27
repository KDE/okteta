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
#include "structuredatainformation.h"

QString StructureDataInformation::typeName() const
{
    return i18nc("data type in C/C++, then name", "struct %1", name());
}

void StructureDataInformation::addDataTypeToStruct(DataInformation* field)
{
    appendChild(field);
}

StructureDataInformation::~StructureDataInformation()
{
}

StructureDataInformation::StructureDataInformation(QString name, DataInformation* parent) :
    DataInformationWithChildren(name, parent)
{
}

StructureDataInformation::StructureDataInformation(const StructureDataInformation& d) :
    DataInformationWithChildren(d)
{
}

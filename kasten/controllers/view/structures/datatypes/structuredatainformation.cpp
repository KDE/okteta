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
#include "structuredatainformation.h"

QString StructureDataInformation::getTypeName() const
{
    return i18nc("data type in C/C++", "struct");
}

int StructureDataInformation::getSize() const
{
    int size = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        size += mChildren[i]->getSize();
    }
    return size;
}

Okteta::Size StructureDataInformation::readData(Okteta::AbstractByteArrayModel* input,
        ByteOrder byteOrder, Okteta::Address address, Okteta::Size remaining)
{
    Okteta::Size readBytes = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        readBytes += mChildren[i]->readData(input, byteOrder, address + readBytes,
                remaining - readBytes);
    }
    return readBytes;
}

void StructureDataInformation::addDataTypeToStruct(DataInformation* field)
{
    appendChild(field);
}

StructureDataInformation& StructureDataInformation::operator<<(DataInformation* field)
{
    if (field)
    {
        addDataTypeToStruct(field);
    }
    return *this;
}

StructureDataInformation::~StructureDataInformation()
{
}

StructureDataInformation::StructureDataInformation(QString name, int index,
        DataInformation* parent) :
    DataInformationWithChildren(name, index, parent)
{
}

StructureDataInformation::StructureDataInformation(const StructureDataInformation& d) :
    DataInformationWithChildren(d)
{
}

StructureDataInformation* StructureDataInformation::fromXML(QDomElement& xmlElem)
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    StructureDataInformation* stru = new StructureDataInformation(name);
    QDomNode node = xmlElem.firstChild();
    while (!node.isNull())
    {
        DataInformation* data = parseNode(node);
        if (data)
            stru->addDataTypeToStruct(data);
        node = node.nextSibling();
    }
    return stru;
}


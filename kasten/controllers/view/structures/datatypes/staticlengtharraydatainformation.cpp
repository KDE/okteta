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
#include "staticlengtharraydatainformation.h"
#include "primitivedatainformation.h"

QString StaticLengthArrayDataInformation::getTypeName() const
{
    if (!hasChildren())
        return i18n("Empty array");

    DataInformation* data = childAt(0);
    if (dynamic_cast<PrimitiveDataInformation*> (data))
    {
        //don't show name of child
        return i18nc("array type then length", "%1[%2]", data->getTypeName(),
                childCount());
    }
    return i18nc("subelem type then array name and length", "%1[%2] (%3)",
            data->getName(), childCount(), data->getTypeName(), data->getName());
}

Okteta::Size StaticLengthArrayDataInformation::readData(
        Okteta::AbstractByteArrayModel* input, ByteOrder byteOrder,
        Okteta::Address address, Okteta::Size remaining)
{
    Okteta::Size readBytes = 0;
    for (unsigned int i = 0; i < childCount(); i++)
    {
        readBytes += childAt(i)->readData(input, byteOrder, address + readBytes,
                remaining - readBytes);
    }
    return readBytes;
}

StaticLengthArrayDataInformation::StaticLengthArrayDataInformation(QString name,
        unsigned int length, const DataInformation& children, int index,
        DataInformation* parent) :
    DataInformationWithChildren(name, index, parent)
{
    //  kDebug() << "name: " << name;
    //  kDebug() << "childname: " << children.getName();
    for (unsigned int i = 0; i < length; i++)
    {
        DataInformation* arrayElem = children.clone();
        QObject::connect(arrayElem, SIGNAL(dataChanged()), this,
                SLOT(onChildDataChanged()));
        appendChild(arrayElem);
    }
}

StaticLengthArrayDataInformation::StaticLengthArrayDataInformation(
        const StaticLengthArrayDataInformation& d) :
    DataInformationWithChildren(d)
{
}

StaticLengthArrayDataInformation::~StaticLengthArrayDataInformation()
{
}

StaticLengthArrayDataInformation*
StaticLengthArrayDataInformation::fromXML(QDomElement& xmlElem)
{
    QString name = xmlElem.attribute("name", i18n("<invalid name>"));
    DataInformation* subElem = NULL;
    QDomNode node = xmlElem.firstChild();
    subElem = parseNode(node);
    if (!subElem)
    {
        kWarning() << "StaticLengthArrayDataInformation::fromXML():"
            " could not parse subelement type";
        return NULL;
    }
    QString lengthStr = xmlElem.attribute("length", QString::null);
    if (lengthStr.isNull())
    {
        kWarning() << "StaticLengthPrimitiveArrayDataInformation::fromXML():"
            " no length attribute defined";
        delete subElem;
        return NULL;
    }
    bool okay = true;
    int length = lengthStr.toInt(&okay, 10); //TODO dynamic length
    if (!okay || length < 0)
    {
        kWarning() << "StaticLengthPrimitiveArrayDataInformation::fromXML():"
            " error parsing length string:" << lengthStr;
        delete subElem;
        return NULL;
    }
    StaticLengthArrayDataInformation* retVal = new StaticLengthArrayDataInformation(name,
            length, *subElem);
    delete subElem; //control not taken over by constructor
    return retVal;
}

Okteta::Size StaticLengthArrayDataInformation::offset(unsigned int index) const
{
    if (index >= childCount())
        return 0;
    Okteta::Size offset = 0;
    //sum size of elements up to index
    for (unsigned int i = 0; i < index; ++i)
    {
        offset += childAt(i)->getSize() / 8;
    }
    return offset;
}


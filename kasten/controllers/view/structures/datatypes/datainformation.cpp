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
#include "datainformation.h"
#include <KLocale>
#include "structuredatainformation.h"
#include "uniondatainformation.h"
#include "primitivedatainformation.h"
#include "staticlengtharraydatainformation.h"
#include "datainformationgroup.h"

DataInformation::DataInformation(const QString& name, int index, DataInformation* parent) :
    QObject(parent), mIndex(index)
{
    setObjectName(name);
}

DataInformation::DataInformation(const DataInformation& d) :
    QObject(NULL), mIndex(d.mIndex)
{
    setObjectName(d.objectName());
}

DataInformation::~DataInformation()
{
}

QString DataInformation::getValueString() const
{
    return QString();
}

QString DataInformation::getSizeString() const
{
    return i18np("1 byte", "%1 bytes", getSize() / 8);
    //  return KGlobal::locale()->formatByteSize(getSize() / 8);

}
DataInformation* DataInformation::parseNode(QDomNode& n)
{
    QDomElement elem = n.toElement(); // try to convert the node to an element.
    DataInformation* data = NULL;
    if (!elem.isNull())
    {
        //      kDebug() << "element tag: " << elem.tagName();
        //e is element
        if (elem.tagName() == "struct")
            data = StructureDataInformation::fromXML(elem);
        else if (elem.tagName() == "array")
            data = StaticLengthArrayDataInformation::fromXML(elem);
        //TODO var length array
        else if (elem.tagName() == "primitive")
            data = PrimitiveDataInformation::fromXML(elem);
        else if (elem.tagName() == "union")
            data = UnionDataInformation::fromXML(elem);
        //FIXME finish these
        //          if (elem.tagName() == "enum")
        //              list.append(EnumDataInformation::fromXML(elem));
        //      kDebug() << "end of tag: " << elem.tagName();

    }
    return data;
}
Okteta::Size DataInformation::positionRelativeToParent() const
{
    DataInformation* par = static_cast<DataInformation*>(parent());
    if (!par || dynamic_cast<DataInformationGroup*>(par)) {
        //TODO remove datainformationgroup
        return 0;
    }
    return par->offset(this->mIndex) + par->positionRelativeToParent();
}


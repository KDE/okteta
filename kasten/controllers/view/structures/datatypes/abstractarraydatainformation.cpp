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
#include "abstractarraydatainformation.h"
#include "primitivedatainformation.h"
#include "dynamiclengtharraydatainformation.h"
#include "staticlengtharraydatainformation.h"

QString AbstractArrayDataInformation::typeName() const
{
    if (!hasChildren())
        return i18n("Empty array");

    DataInformation* data = childAt(0);
    if (dynamic_cast<PrimitiveDataInformation*> (data))
    {
        //don't show name of child
        return i18nc("array type then length", "%1[%2]", data->typeName(),
                childCount());
    }
    return i18nc("subelem type then array name and length", "%1[%2] (%3)",
            data->name(), childCount(), data->typeName());
}

AbstractArrayDataInformation::AbstractArrayDataInformation(QString name,
        const DataInformation& childType, uint length, int index,
        DataInformation* parent) :
    DataInformationWithChildren(name, index, parent), mChildType(0)
{
    mChildType = childType.clone();
    for (unsigned int i = 0; i < length; i++)
    {
        DataInformation* arrayElem = childType.clone();
        QObject::connect(arrayElem, SIGNAL(dataChanged()), this,
                SIGNAL(dataChanged()));
        appendChild(arrayElem);
    }
}

AbstractArrayDataInformation::AbstractArrayDataInformation(
        const AbstractArrayDataInformation& d) :
    DataInformationWithChildren(d), mChildType(0)
{
    if (d.mChildType)
        mChildType = d.mChildType->clone();
}

bool AbstractArrayDataInformation::isDynamicArray() const
{
    //if this array has an update function it must be dynamic
    return mAdditionalData->updateFunction() != NULL;
}

void AbstractArrayDataInformation::setArrayLength(int newLength)
{
    //kDebug() << "old childcount: " << childCount();

    //kDebug() << "newLength: " << newLength;
    if (newLength < 0)
        return;
    if (newLength > 1000000)
    {
        kWarning() << "attempting to set the length of the array" << name() << "to "
                << newLength << " which would use too much memory";
        return;
    }
    if (newLength > childCount())
    {
        emit childCountChange(childCount(), newLength);
        for (uint i = childCount(); i < newLength; ++i)
        {
            DataInformation* arrayElem = mChildType->clone();
            QObject::connect(arrayElem, SIGNAL(dataChanged()), this,
                    SIGNAL(dataChanged()));
            appendChild(arrayElem);
        }
    }
    else if (newLength < childCount()) //TODO maybe keep some cached
    {
        emit childCountChange(childCount(), newLength);
        for (int i = newLength; i != mChildren.length();)
        {
            delete mChildren.takeAt(i);
        }
    }
}

void AbstractArrayDataInformation::setArrayType(QScriptValue type)
{
    //FIXME stub
}

AbstractArrayDataInformation::~AbstractArrayDataInformation()
{
    delete mChildType;
}

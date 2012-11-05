/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
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

#include "topleveldatainformation.h"

#include <KLocalizedString>

QString StructureDataInformation::typeName() const
{
    return i18nc("data type in C/C++, then name", "struct %1", name());
}

StructureDataInformation::~StructureDataInformation()
{
}

StructureDataInformation::StructureDataInformation(const QString& name,
        const QVector<DataInformation*>& children, DataInformation* parent)
        : DataInformationWithChildren(name, children, parent)
{
}

qint64 StructureDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    TopLevelDataInformation* top = topLevelDataInformation();
    Q_CHECK_PTR(top);

    uint readBytes = 0;
    qint64 readBits = 0;
    const quint8 origBitOffset = *bitOffset;
    for (int i = 0; i < mChildren.size(); i++)
    {
        DataInformation* next = mChildren.at(i);
        top->scriptHandler()->updateDataInformation(next);
        //next may be a dangling pointer now, reset it
        DataInformation* newNext = mChildren.at(i);
        if (next != newNext)
        {
            logInfo() << "Child at index " << i << " was replaced.";
            top->setChildDataChanged();
        }
        qint64 currentReadBits = newNext->readData(input, address + readBytes,
                bitsRemaining - readBits, bitOffset);
        if (currentReadBits == -1)
        {
            mWasAbleToRead = false;
            //could not read one element -> whole structure could not be read
            return -1;
        }
        readBits += currentReadBits;
        readBytes = (readBits + origBitOffset) / 8;
    }
    mWasAbleToRead = true;
    return readBits;
}

BitCount64 StructureDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    BitCount64 offset = 0;
    //sum size of elements up to index
    for (int i = 0; i < mChildren.size(); ++i)
    {
        DataInformation* current = mChildren.at(i);
        if (current == child)
            break;
        offset += current->size();
    }

    if (mParent->isTopLevel())
        return start * 8 + offset;
    else
        return mParent->asDataInformation()->childPosition(this, start) + offset;
}

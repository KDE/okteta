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
#include "uniondatainformation.h"
#include "topleveldatainformation.h"

#include <KLocale>

QString UnionDataInformation::typeName() const
{
    return i18nc("data type in C/C++, then name", "union %1", name());
}

BitCount32 UnionDataInformation::size() const
{
    //since this is a union return size of biggest element
    BitCount32 size = 0;
    for (int i = 0; i < mChildren.size(); i++)
    {
        size = qMax(size, mChildren.at(i)->size());
    }
    return size;
}

qint64 UnionDataInformation::readData(Okteta::AbstractByteArrayModel *input,
        Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); //update must have been called prior to reading
    TopLevelDataInformation* top = topLevelDataInformation();
    Q_CHECK_PTR(top);

    qint64 readBits = 0;
    const quint8 originalBitOffset = *bitOffset;
    quint8 bitOffsetAfterUnion = originalBitOffset;
    bool reachedEOF = false;
    for (int i = 0; i < mChildren.size(); i++)
    {
        DataInformation* next = mChildren.at(i);
        //first of all update the structure:
        top->scriptHandler()->updateDataInformation(next);
        DataInformation* newNext = mChildren.at(i);
        if (next != newNext)
        {
            logInfo() << "Child at index " << i << " was replaced.";
            top->setChildDataChanged();
        }
        //bit offset always has to be reset to original value
        qint64 currentReadBits = newNext->readData(input, address, bitsRemaining, bitOffset);
        if (currentReadBits == -1)
        {
            //since this is a union, try to read all values and not abort as soon as one is too large
            reachedEOF = true;
        }
        else if (currentReadBits > readBits)
        {
            //this is the largest element, so the bit offset after the union is the one after this element
            readBits = currentReadBits;
            bitOffsetAfterUnion = *bitOffset;
        }
        *bitOffset = originalBitOffset; // start at beginning
    }
    *bitOffset = bitOffsetAfterUnion;
    mWasAbleToRead = !reachedEOF;
    return reachedEOF ? -1 : readBits;
}

UnionDataInformation::~UnionDataInformation()
{
}

UnionDataInformation::UnionDataInformation(const QString& name, const QVector<DataInformation*>& children,
        DataInformation* parent)
        : DataInformationWithChildren(name, children, parent)
{
}

BitCount64 UnionDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    //all elements start at same position
    if (mParent->isTopLevel())
        return start * 8;
    else
        return mParent->asDataInformation()->childPosition(this, start);
}


/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuredatainformation.hpp"

#include "topleveldatainformation.hpp"

#include <KLocalizedString>
#include <limits>

StructureDataInformation::StructureDataInformation(const QString& name,
                                                   const QVector<DataInformation*>& children, DataInformation* parent)
    : DataInformationWithChildren(name, children, parent)
{
}

StructureDataInformation::~StructureDataInformation() = default;

QString StructureDataInformation::typeNameImpl() const
{
    return i18nc("data type in C/C++, then name", "struct %1", name());
}

bool StructureDataInformation::isStruct() const
{
    return true;
}

qint64 StructureDataInformation::readData(Okteta::AbstractByteArrayModel* input,
                                          Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading
    qint64 bitsRead = 0;
    mWasAbleToRead = readChildren(mChildren, input, address, bitsRemaining, bitOffset, &bitsRead, topLevelDataInformation());
    return bitsRead;
}

BitCount64 StructureDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    BitCount64 offset = 0;
    // sum size of elements up to index
    for (auto* current : mChildren) {
        if (current == child) {
            break;
        }
        offset += current->size();
    }

    if (mParent->isTopLevel()) {
        return start * 8 + offset;
    }

    return mParent->asDataInformation()->childPosition(this, start) + offset;
}

bool StructureDataInformation::readChildren(const QVector<DataInformation*>& children,
                                            Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining,
                                            quint8* bitOffset, qint64* readBitsPtr, TopLevelDataInformation* top)
{
    Q_CHECK_PTR(top);
    Q_CHECK_PTR(readBitsPtr);
    Q_ASSERT(*readBitsPtr >= 0); // otherwise we failed before
    qint64 readBits = *readBitsPtr;
    // prevent overflow
    Q_ASSERT(sizeof(qint64) == sizeof(Okteta::Address) || readBits < (qint64(std::numeric_limits<qint32>::max()) * 8));
    const quint8 origBitOffset = *bitOffset;
    Okteta::Address readBytes = (readBits + origBitOffset) / 8;
    for (int i = 0; i < children.size(); i++) {
        DataInformation* next = children.at(i);
        top->scriptHandler()->updateDataInformation(next);
        // next may be a dangling pointer now, reset it
        DataInformation* newNext = children.at(i);
        if (next != newNext) {
            // logInfo() << "Child at index " << i << " was replaced.";
            top->setChildDataChanged();
        }
        qint64 currentReadBits = newNext->readData(input, address + readBytes,
                                                   bitsRemaining - readBits, bitOffset);
        if (currentReadBits == -1) {
            *readBitsPtr = -1;
            return false; // could not read one element -> whole structure could not be read
        }
        readBits += currentReadBits;
        readBytes = (readBits + origBitOffset) / 8;
    }

    *readBitsPtr = readBits;
    return true;
}

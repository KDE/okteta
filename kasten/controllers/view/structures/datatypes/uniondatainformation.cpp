/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2010 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "uniondatainformation.hpp"
#include "topleveldatainformation.hpp"

#include <KLocalizedString>

UnionDataInformation::UnionDataInformation(const QString& name, const QVector<DataInformation*>& children,
                                           DataInformation* parent)
    : DataInformationWithChildren(name, children, parent)
{
}

UnionDataInformation::~UnionDataInformation() = default;

QString UnionDataInformation::typeNameImpl() const
{
    return i18nc("data type in C/C++, then name", "union %1", name());
}

bool UnionDataInformation::isUnion() const
{
    return true;
}

BitCount32 UnionDataInformation::size() const
{
    // since this is a union return size of biggest element
    BitCount32 size = 0;
    for (auto* child : mChildren) {
        size = qMax(size, child->size());
    }

    return size;
}

qint64 UnionDataInformation::readData(Okteta::AbstractByteArrayModel* input,
                                      Okteta::Address address, BitCount64 bitsRemaining, quint8* bitOffset)
{
    Q_ASSERT(mHasBeenUpdated); // update must have been called prior to reading
    TopLevelDataInformation* top = topLevelDataInformation();
    Q_CHECK_PTR(top);

    qint64 readBits = 0;
    const quint8 originalBitOffset = *bitOffset;
    quint8 bitOffsetAfterUnion = originalBitOffset;
    bool reachedEOF = false;
    for (int i = 0; i < mChildren.size(); i++) {
        DataInformation* next = mChildren.at(i);
        // first of all update the structure:
        top->scriptHandler()->updateDataInformation(next);
        DataInformation* newNext = mChildren.at(i);
        if (next != newNext) {
            logInfo() << "Child at index " << i << " was replaced.";
            top->setChildDataChanged();
        }
        // bit offset always has to be reset to original value
        qint64 currentReadBits = newNext->readData(input, address, bitsRemaining, bitOffset);
        if (currentReadBits == -1) {
            // since this is a union, try to read all values and not abort as soon as one is too large
            reachedEOF = true;
        } else if (currentReadBits > readBits) {
            // this is the largest element, so the bit offset after the union is the one after this element
            readBits = currentReadBits;
            bitOffsetAfterUnion = *bitOffset;
        }
        *bitOffset = originalBitOffset; // start at beginning
    }

    *bitOffset = bitOffsetAfterUnion;
    mWasAbleToRead = !reachedEOF;
    return reachedEOF ? -1 : readBits;
}

BitCount64 UnionDataInformation::childPosition(const DataInformation* child, Okteta::Address start) const
{
    // all elements start at offset zero
    Q_UNUSED(child)
    if (mParent->isTopLevel()) {
        return start * 8;
    }

    return mParent->asDataInformation()->childPosition(this, start);
}

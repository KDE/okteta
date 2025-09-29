/*
    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuredatainformation.hpp"

// lib
#include "topleveldatainformation.hpp"
// KF
#include <KLocalizedString>
// Std
#include <limits>
#include <utility>

StructureDataInformation::StructureDataInformation(const QString& name,
                                                   std::vector<std::unique_ptr<DataInformation>>&& children,
                                                   DataInformation* parent)
    : DataInformationWithChildren(name, std::move(children), parent)
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

qint64 StructureDataInformation::readData(const Okteta::AbstractByteArrayModel* input,
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
    for (const auto& current : mChildren) {
        if (current.get() == child) {
            break;
        }
        offset += current->size();
    }

    if (mParent->isTopLevel()) {
        return start * 8 + offset;
    }

    return mParent->asDataInformation()->childPosition(this, start) + offset;
}

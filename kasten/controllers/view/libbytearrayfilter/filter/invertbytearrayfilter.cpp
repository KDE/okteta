/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "invertbytearrayfilter.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

InvertByteArrayFilter::InvertByteArrayFilter()
    : AbstractByteArrayFilter(
        i18nc("name of the filter; it switches all bits from 0 to 1 and 1 to 0 respectively, so 01111110 becomes 10000001",
              "INVERT data"))
{}

InvertByteArrayFilter::~InvertByteArrayFilter() = default;

AbstractByteArrayFilterParameterSet* InvertByteArrayFilter::parameterSet() { return &mNoParameterSet; }

bool InvertByteArrayFilter::filter(Okteta::Byte* result,
                                   Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    int r = 0;
    Okteta::Address m = range.start();
    int nextBlockEnd = FilteredByteCountSignalLimit;
    while (m <= range.end()) {
        result[r++] = ~model->byte(m++);

        if (r >= nextBlockEnd) {
            nextBlockEnd += FilteredByteCountSignalLimit;
            emit filteredBytes(r);
        }
    }

    return true;
}

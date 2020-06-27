/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "reversebytearrayfilter.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>

ReverseByteArrayFilter::ReverseByteArrayFilter()
    : AbstractByteArrayFilter(
        i18nc("name of the filter; it changes the order of the bytes/bits to backwards, so ABCD becomes DCBA",
              "REVERSE data"))
{}

ReverseByteArrayFilter::~ReverseByteArrayFilter() = default;

AbstractByteArrayFilterParameterSet* ReverseByteArrayFilter::parameterSet() { return &mParameterSet; }

bool ReverseByteArrayFilter::filter(Okteta::Byte* result,
                                    Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    Okteta::Size r = range.width() - 1;
    Okteta::Address m = range.start();
    int filteredBytesCount = 0;
    while (m <= range.end()) {
        Okteta::Byte byte = model->byte(m++);
        Okteta::Byte reverseByte;
        if (mParameterSet.invertsBits()) {
            reverseByte = 0;
            for (int b = 7; b >= 0; --b) {
                reverseByte |= (byte & 0x80) >> b;
                byte <<= 1;
            }
        } else {
            reverseByte = byte;
        }
        result[r--] = reverseByte;

        ++filteredBytesCount;
        if (filteredBytesCount >= FilteredByteCountSignalLimit) {
            filteredBytesCount = 0;
            emit filteredBytes(m - range.start());
        }
    }

    return true;
}

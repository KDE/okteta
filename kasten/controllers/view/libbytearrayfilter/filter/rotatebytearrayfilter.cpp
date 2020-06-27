/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "rotatebytearrayfilter.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QtGlobal>
// Std
#include <cstdlib>

static constexpr int RotateBitsPerByte = 8;

RotateByteArrayFilter::RotateByteArrayFilter()
    : AbstractByteArrayFilter(
        i18nc("name of the filter; it moves the bits and pushes the ones over the end to the begin again",
              "ROTATE data"))
{}

RotateByteArrayFilter::~RotateByteArrayFilter() = default;

AbstractByteArrayFilterParameterSet* RotateByteArrayFilter::parameterSet() { return &mParameterSet; }

bool RotateByteArrayFilter::filter(Okteta::Byte* result,
                                   Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const int groupSize = mParameterSet.groupSize();
    const int groupBitCount = (groupSize * RotateBitsPerByte);
    const int groupShiftBitWidth = std::abs(mParameterSet.moveBitWidth()) % groupBitCount;

    const int shiftByteWidth = groupShiftBitWidth / RotateBitsPerByte;
    const int shiftBitWidth = groupShiftBitWidth - shiftByteWidth * RotateBitsPerByte;
    const int otherShiftBitWidth = RotateBitsPerByte - shiftBitWidth;
    int filteredBytesCount = 0;

    const bool toRight = (mParameterSet.moveBitWidth() > 0);
    if (toRight) {
        int r = 0;
        Okteta::Address m = range.start();
        while (m <= range.end()) {
            int g = 0;
            // round the edge byte layer shift
            while (g < shiftByteWidth && m + groupSize <= range.end()) {
                result[r++] = model->byte((m++) + groupSize - shiftByteWidth);
                ++g;
            }
            // normal byte layer shift
            while (g < groupSize && m <= range.end()) {
                result[r++] = model->byte((m++) - shiftByteWidth);
                ++g;
            }

            // bit layer shift
            const unsigned char last = (unsigned char) result[r - 1];
            for (int b = 1; b <= g; ++b) {
                result[r - b] = (unsigned char)result[r - b] >> shiftBitWidth;
                if (b < g) {
                    result[r - b] |= (unsigned char)result[r - b - 1] << otherShiftBitWidth;
                } else if (g == groupSize) {
                    result[r - b] |= last << otherShiftBitWidth;
                }
            }

            filteredBytesCount += g;
            if (filteredBytesCount >= FilteredByteCountSignalLimit) {
                filteredBytesCount = 0;
                emit filteredBytes(m - range.start());
            }
        }
    } else {
        int r = 0;
        Okteta::Address m = range.start();
        while (m <= range.end()) {
            int g = 0;
            // normal byte layer shift
            while (g + shiftByteWidth < groupSize && m + shiftByteWidth <= range.end()) {
                result[r++] = model->byte((m++) - shiftByteWidth);
                ++g;
            }
            // round the edge byte layer shift
            while (g < groupSize && m <= range.end()) {
                result[r++] = model->byte((m++) + shiftByteWidth - groupSize);
                ++g;
            }

            // bit layer shift
            const unsigned char first = result[r - g];
            for (int b = g; b > 0; --b) {
                result[r - b] = (unsigned char)result[r - b] << shiftBitWidth;
                if (b > 1) {
                    result[r - b] |= (unsigned char)result[r - b + 1] >> otherShiftBitWidth;
                } else if (g == groupSize) {
                    result[r - b] |= first >> otherShiftBitWidth;
                }
            }

            filteredBytesCount += g;
            if (filteredBytesCount >= FilteredByteCountSignalLimit) {
                filteredBytesCount = 0;
                emit filteredBytes(m - range.start());
            }
        }
    }

    return true;
}

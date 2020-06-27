/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "shiftbytearrayfilter.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QtGlobal>
// Std
#include <cstdlib>

// TODO: add option which bit (0/1) to insert
static constexpr int ShiftBitsPerByte = 8;

ShiftByteArrayFilter::ShiftByteArrayFilter()
    : AbstractByteArrayFilter(
        i18nc("name of the filter; it moves the bits, setting freed ones to zero",
              "SHIFT data"))
{}

ShiftByteArrayFilter::~ShiftByteArrayFilter() = default;

AbstractByteArrayFilterParameterSet* ShiftByteArrayFilter::parameterSet() { return &mParameterSet; }

bool ShiftByteArrayFilter::filter(Okteta::Byte* result,
                                  Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const
{
    const int groupSize = mParameterSet.groupSize();
    const int groupBitCount = (groupSize * ShiftBitsPerByte);
    const int groupShiftBitWidth = std::abs(mParameterSet.moveBitWidth()) % groupBitCount;

    const int shiftByteWidth = groupShiftBitWidth / ShiftBitsPerByte;
    const int shiftBitWidth = groupShiftBitWidth - shiftByteWidth * ShiftBitsPerByte;
    const int otherShiftBitWidth = ShiftBitsPerByte - shiftBitWidth;
    int filteredBytesCount = 0;

    const bool toRight = (mParameterSet.moveBitWidth() > 0);
    if (toRight) {
        int r = 0;
        Okteta::Address m = range.start();
        while (m <= range.end()) {
            int g = 0;
            // full free bytes
            while (g < shiftByteWidth && m <= range.end()) {
                result[r++] = 0;
                ++m;
                ++g;
            }
            // byte layer shift
            while (g < groupSize && m <= range.end()) {
                result[r++] = model->byte((m++) - shiftByteWidth);
                ++g;
            }
            // bit layer shift
            for (int b = 1; b <= g; ++b) {
                result[r - b] = (unsigned char)result[r - b] >> shiftBitWidth;
                if (b < g) {
                    result[r - b] |= (unsigned char)result[r - b - 1] << otherShiftBitWidth;
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
            // byte layer shift
            while (g + shiftByteWidth < groupSize && m + shiftByteWidth <= range.end()) {
                result[r++] = model->byte((m++) + shiftByteWidth);
                ++g;
            }
            // full free bytes
            while (g < groupSize && m <= range.end()) {
                result[r++] = 0;
                ++m;
                ++g;
            }

            // bit layer shift
            for (int b = g; b > 0; --b) {
                result[r - b] = (unsigned char)result[r - b] << shiftBitWidth;
                if (b > 1) {
                    result[r - b] |= (unsigned char)result[r - b + 1] >> otherShiftBitWidth;
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

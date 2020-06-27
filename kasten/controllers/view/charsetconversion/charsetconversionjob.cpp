/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charsetconversionjob.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
#include <Okteta/Character>
// Qt
#include <QCoreApplication>

namespace Kasten {

static constexpr int CharsetConversionBlockSize = 100000;

bool CharsetConversionJob::exec()
{
    // reset
    mConvertedBytesCount = 0;
    mFailedPerByteCount.clear();

    // check
    if (!mByteArrayModel || !mRange.isValid()) {
        deleteLater(); // TODO: could be reused on next operation

        return false;
    }

    bool success = true;
    int r = 0;
    Okteta::Address i = mRange.start();
    Okteta::Address blockEnd = mRange.start();
    while (i <= mRange.end() && success) {
        blockEnd += CharsetConversionBlockSize;
        if (blockEnd > mRange.end()) {
            blockEnd = mRange.end();
        }

        for (; i <= blockEnd && success; ++i) {
            Okteta::Byte convertedByte;
            const Okteta::Byte originalByte = mByteArrayModel->byte(i);
            const Okteta::Character decodedChar = mSourceCharCodec->decode(originalByte);
            bool isConverted = !decodedChar.isUndefined();
            if (isConverted) {
                isConverted = mTargetCharCodec->encode(&convertedByte, decodedChar);
            }

            if (!isConverted) {
                if (!mSubstitutingMissingChars) {
                    success = false;
                    break;
                }
                mFailedPerByteCount[originalByte]++;
                convertedByte = mSubstituteByte;
            }

            // TODO: means if the default byte equals a not convertable original
            // byte that one is not counted. okay, or should be made explicit to user?
            if (originalByte != convertedByte) {
                mConvertedBytesCount++;
            }

            mResult[r++] = convertedByte;
        }

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    deleteLater(); // TODO: could be reused on next operation

    return success;
}

}

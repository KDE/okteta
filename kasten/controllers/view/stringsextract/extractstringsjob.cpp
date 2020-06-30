/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "extractstringsjob.hpp"

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/CharCodec>
#include <Okteta/Character>
// Qt
#include <QCoreApplication>

namespace Kasten {

static constexpr int ExtractStringBlockSize = 100000;

void ExtractStringsJob::exec()
{

    // reset
    mContainedStringList->clear();

    // check
    if (!mByteArrayModel || !mSelection.isValid()) {
        deleteLater(); // TODO: could be reused on next operation

        return;
    }

    bool stringStarted = false;
    Okteta::Address stringStart = mSelection.start();
    QString string;

    Okteta::Address i = mSelection.start();
    Okteta::Address blockEnd = mSelection.start();
    while (i <= mSelection.end()) {
        blockEnd += ExtractStringBlockSize;
        if (blockEnd > mSelection.end()) {
            blockEnd = mSelection.end();
        }

        for (; i <= blockEnd; ++i) {
            const Okteta::Character decodedChar = mCharCodec->decode(mByteArrayModel->byte(i));
            // TODO: Zeilenumbrüche ausnehmen
            const bool isStringChar = (!decodedChar.isUndefined() &&
                                       (decodedChar.isLetterOrNumber() || decodedChar.isSpace() || decodedChar.isPunct()));

            if (isStringChar) {
                if (!stringStarted) {
                    stringStart = i;
                    stringStarted = true;
                    string.clear();
                }
                string.append(decodedChar);
            } else {
                if (stringStarted) {
                    if (i - stringStart >= mMinLength) {
                        mContainedStringList->append(ContainedString(string, stringStart));
                    }
                    stringStarted = false;
                }
            }
        }

        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    // last string not ended?
    if (stringStarted) {
        if (i - stringStart >= mMinLength) {
            mContainedStringList->append(ContainedString(string, stringStart));
        }
        stringStarted = false;
    }

    deleteLater(); // TODO: could be reused on next operation
}

}

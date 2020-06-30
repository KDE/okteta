/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "utf32stringdata.hpp"

#include "../../structlogging.hpp"

#include <QVarLengthArray>

#include <KLocalizedString>

#include <Okteta/AbstractByteArrayModel>

#include "../topleveldatainformation.hpp"
#include "../dummydatainformation.hpp"
#include "stringdatainformation.hpp"

Utf32StringData::Utf32StringData(StringDataInformation* parent)
    : StringData(parent)
{
}

Utf32StringData::~Utf32StringData() = default;

QString Utf32StringData::charType() const
{
    return mLittleEndian ? i18n("UTF32-LE char") : i18n("UTF32-BE char");
}

QString Utf32StringData::typeName() const
{
    return mLittleEndian ? i18n("UTF32-LE string") : i18n("UTF32-BE string");
}

uint Utf32StringData::count() const
{
    return mCodePoints.size();
}

QString Utf32StringData::stringValue(int row) const
{
    // TODO details
    Q_ASSERT((uint)row < count());
    // TODO show invalid values
    uint val = mCodePoints.at(row);
    QString number = QString::number(val, 16).toUpper();
    if (number.length() == 1) {
        number = QLatin1Char('0') + number;
    }
    if (val > UNICODE_MAX) {
        return i18n("Value too big: 0x%1", number);
    }
    if (val > BMP_MAX) {
        QString ret(2, Qt::Uninitialized);
        ret[0] = QChar::highSurrogate(val);
        ret[1] = QChar::lowSurrogate(val);
        return i18n("%1 (U+%2)", ret, number);
    }

    return i18n("%1 (U+%2)", QString(QChar(mCodePoints.at(row))), number);
}

QString Utf32StringData::completeString(bool skipInvalid) const
{
    QVarLengthArray<QChar> data(mCodePoints.size() + mNonBMPCount);
    int codePointCount = mCodePoints.size();
    int i = 0;
    for (int idx = 0; idx < codePointCount; ++idx) {
        uint val = mCodePoints.at(idx);
        if (val > UNICODE_MAX) {
            if (skipInvalid) {
                continue;
            }

            data[i] = QChar::ReplacementCharacter;
        } else if (val > BMP_MAX) {
            data[i] = QChar::highSurrogate(val);
            i++;
            data[i] = QChar::lowSurrogate(val);
        } else {
            data[i] = QChar((ushort)val);
        }
        i++;
    }

    return QString(data.constData(), i);
}

qint64 Utf32StringData::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
                             BitCount64 bitsRemaining)
{
    const int oldSize = count();
    mNonBMPCount = 0;
    if (mMode == CharCount) {
        mCodePoints.reserve(mLength.maxChars);
    } else if (mMode == ByteCount) {
        mCodePoints.reserve(mLength.maxBytes / 4);
    }

    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, oldSize, 0);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, oldSize, 0);

    const uint oldMax = mCodePoints.size();
    quint64 remaining = bitsRemaining;
    Okteta::Address addr = address;
    uint count = 0;
    mEofReached = false;
    if (((mMode & CharCount) && mLength.maxChars == 0)
        || ((mMode & ByteCount) && mLength.maxBytes < 2)) {
        return 0;
    }

    bool eofAtStart = false;
    if (remaining < 32) {
        eofAtStart = true;
    }

    while (true) {
        if (remaining < 32) {
            mEofReached = true;
            break;
        }
        uint codePoint;
        bool terminate = false;

        if (mLittleEndian) {
            codePoint = input->byte(addr) | (input->byte(addr + 1) << 8)
                        | (input->byte(addr + 2) << 16) | (input->byte(addr + 3) << 24);
        } else {
            codePoint = (input->byte(addr) << 24) | (input->byte(addr + 1) << 16)
                        | (input->byte(addr + 2) << 8) | input->byte(addr + 3);
        }

        if (count < oldMax) {
            mCodePoints[count] = codePoint;
        } else {
            mCodePoints.append(codePoint);
        }

        remaining -= 32;
        addr += 4;
        count++;

        // now check if we have to terminate
        if (mMode & Sequence) {
            if (codePoint == mTerminationCodePoint) {
                terminate = true;
            }
        }
        if (mMode & ByteCount) {
            // divide by two in case someone set length to an odd number of bytes
            if ((addr - address) / 4 >= Okteta::Address(mLength.maxBytes / 4)) {
                terminate = true;
            }
        }
        if (mMode & CharCount) {
            if (count >= mLength.maxChars) {
                terminate = true;
            }
        }
        if (mMode == None) {
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "no termination mode set!!";
            Q_ASSERT(false);
        }
        if (terminate) {
            break;
        }
    }
    mCodePoints.resize(count);
    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, 0, count);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, 0, count);

    if (eofAtStart) {
        return -1;
    }
    return (addr - address) * 8;
}

BitCount32 Utf32StringData::size() const
{
    return mCodePoints.size() * 32;
}

BitCount32 Utf32StringData::sizeAt(uint i) const
{
    Q_ASSERT(i <= count());
    Q_UNUSED(i)
    return 32;
}

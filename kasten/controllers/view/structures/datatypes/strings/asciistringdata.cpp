/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "asciistringdata.hpp"
#include "stringdatainformation.hpp"
#include "../topleveldatainformation.hpp"
#include "../../structlogging.hpp"

#include <Okteta/AbstractByteArrayModel>

#include <KLocalizedString>
#include <QVarLengthArray>

AsciiStringData::AsciiStringData(StringDataInformation* parent)
    : StringData(parent)
{
}

AsciiStringData::~AsciiStringData() = default;

qint64 AsciiStringData::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
{
    const int oldSize = count();
    if (mMode == CharCount || mMode == ByteCount) { // same for ascii
        mData.reserve(mLength.maxChars);
    }
    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, oldSize, 0);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, oldSize, 0);

    quint64 remaining = bitsRemaining;
    Okteta::Address addr = address;
    int count = 0;
    mEofReached = false;
    const int oldMax = mData.size();
    if (((mMode & CharCount) && mLength.maxChars == 0) || ((mMode & ByteCount) && mLength.maxBytes == 0)) {
        return 0; // nothing to read

    }
    bool eofAtStart = false;
    if (bitsRemaining < 8) {
        eofAtStart = true;
    }

    while (true) {
        if (remaining < 8) {
            mEofReached = true;
            break;
        }
        uchar val = input->byte(addr);
        bool terminate = false;

        if (count < oldMax) {
            mData[count] = val;
        } else {
            mData.append(val);
        }

        remaining -= 8;
        addr++;
        count++;

        // now check if we have to terminate
        if (mMode & Sequence) {
            if ((quint32(val) & 0xff) == mTerminationCodePoint) {
                terminate = true;
            }
        }
        if ((mMode & CharCount)  || (mMode & ByteCount)) {
            if ((unsigned)count >= mLength.maxChars) {
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
    mData.resize(count);
    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, 0, count);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, 0, count);

    if (eofAtStart) {
        return -1;
    }
    return (addr - address) * 8;
}

BitCount32 AsciiStringData::sizeAt(uint i) const
{
    Q_ASSERT(i < count());
    Q_UNUSED(i)
    return 8;
}

BitCount32 AsciiStringData::size() const
{
    return mData.size() * 8;
}

QString AsciiStringData::completeString(bool skipInvalid) const
{
    int max = mData.size();
    QVarLengthArray<QChar> buf(max);
    for (int i = 0; i < max; ++i) {
        uchar val = mData.at(i);
        if (val > ASCII_MAX) {
            if (skipInvalid) {
                max--;
                i--;
                continue;
            }

            buf[i] = QChar::ReplacementCharacter;
        } else {
            buf[i] = QChar::fromLatin1(val);
        }
    }

    return QString(buf.constData(), max);
}

QString AsciiStringData::stringValue(int row) const
{
    Q_ASSERT(row >= 0 && row < mData.size());
    uchar val = mData.at(row);
    if (val > ASCII_MAX) {
        return i18n("Non-ASCII char: 0x%1", val);
    }

    return QChar::fromLatin1(val);
}

QString AsciiStringData::charType() const
{
    return i18n("ASCII char");
}

uint AsciiStringData::count() const
{
    return mData.size();
}

QString AsciiStringData::typeName() const
{
    return i18n("ASCII string");
}

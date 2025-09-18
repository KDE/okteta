/*
    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2016 Aaron Bishop <erroneous@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "ebcdicstringdata.hpp"

// lib
#include "stringdatainformation.hpp"
#include <topleveldatainformation.hpp>
#include <structureslogging.hpp>
// Okteta core sources
#include <charcodecs/ebcdic1047charcodec.hpp>
// Okteta core
#include <Okteta/Character>
#include <Okteta/AbstractByteArrayModel>
// KF
#include <KLocalizedString>
// Qt
#include <QVarLengthArray>

EbcdicStringData::EbcdicStringData(StringDataInformation* parent)
    : StringData(parent)
    , mCodec(Okteta::EBCDIC1047CharCodec::create())
{
}

EbcdicStringData::~EbcdicStringData() = default;

qint64 EbcdicStringData::read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
{
    const int oldSize = count();
    if (mMode == CharCount || mMode == ByteCount) { // same for ebcdic
        mData.reserve(mLength.maxChars);
    }
    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, oldSize, 0);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, oldSize, 0);

    quint64 remaining = bitsRemaining;
    Okteta::Address addr = address;
    std::size_t count = 0;
    mEofReached = false;
    const std::size_t oldMax = mData.size();
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
            mData.emplace_back(val);
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

    mData.resize(count);
    mParent->topLevelDataInformation()->_childCountAboutToChange(mParent, 0, count);
    mParent->topLevelDataInformation()->_childCountChanged(mParent, 0, count);

    if (eofAtStart) {
        return -1;
    }
    return (addr - address) * 8;
}

BitCount32 EbcdicStringData::sizeAt(uint i) const
{
    Q_ASSERT(i < count());
    Q_UNUSED(i)
    return 8;
}

BitCount32 EbcdicStringData::size() const
{
    return mData.size() * 8;
}

QString EbcdicStringData::completeString() const
{
    const std::size_t max = mData.size();
    QVarLengthArray<QChar> buf(max);
    for (std::size_t i = 0; i < max; ++i) {
        const uchar val = mData[i];
        buf[i] = mCodec->decode(val);
    }

    return QString(buf.constData(), max);
}

QString EbcdicStringData::stringValue(int row) const
{
    Q_ASSERT(0 <= row && row < static_cast<int>(mData.size()));
    const uchar val = mData[row];
    return mCodec->decode(val);
}

QString EbcdicStringData::charType() const
{
    return i18n("EBCDIC char");
}

uint EbcdicStringData::count() const
{
    return mData.size();
}

QString EbcdicStringData::typeName() const
{
    return i18n("EBCDIC string");
}

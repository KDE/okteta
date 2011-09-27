/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2011 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */



#include "utf8stringdata.h"

#include <QVarLengthArray>
#include <abstractbytearraymodel.h>

#include "../topleveldatainformation.h"
#include "../dummydatainformation.h"
#include "stringdatainformation.h"

Utf8StringData::Utf8StringData(StringDataInformation* parent)
    : StringData(parent), mOneByteCount(0), mTwoByteCount(0), mThreeByteCount(0), mFourByteCount(0), mNonBMPCount(0)
{
}

Utf8StringData::~Utf8StringData()
{
}

QString Utf8StringData::charType() const
{
    return i18n("UTF8 char");
}

QString Utf8StringData::typeName() const
{
    return i18n("UTF8 string");
}

int Utf8StringData::count() const
{
    return mCodePoints.size();
}

QString Utf8StringData::stringValue(int row) const
{
    Q_ASSERT(row >= 0 && row < count());
    //TODO show invalid values
    uint val = mCodePoints.at(row);
    QString number = QString::number(val, 16).toUpper();
    if (number.length() == 1)
        number = QLatin1String("0") + number;
    if (val > UNICODE_MAX)
        return i18n("Value too big: 0x%1", number);
    else if (val > BMP_MAX) {
        QString ret(2, Qt::Uninitialized);
        ret[0] = QChar::highSurrogate(val);
        ret[1] = QChar::lowSurrogate(val);
        return i18n("%1 (U+%2)", ret, number);
    }
    else
        return i18n("%1 (U+%2)", QString(QChar(mCodePoints.at(row))), number);
}

QString Utf8StringData::completeString(bool skipInvalid) const
{
    QVarLengthArray<QChar> data(mCodePoints.size() + mNonBMPCount);
    int codePointCount = mCodePoints.size();
    int i = 0;
    for (int idx = 0; idx < codePointCount; ++idx) {
        uint val = mCodePoints.at(idx);
        //if error at idx is set also skip
        if (val > UNICODE_MAX || mErrorIndices.value(idx))
        {
            if (skipInvalid)
                continue;
            else
                data[i] = QChar::ReplacementCharacter;
        }
        else if (val > BMP_MAX) {
            data[i] = QChar::highSurrogate(val);
            i++;
            data[i] = QChar::lowSurrogate(val);
        }
        else
        {
            data[i] = QChar((ushort)val);
        }
        i++;
    }
    return QString(data.constData(), i);
}

qint64 Utf8StringData::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address,
            BitCount64 bitsRemaining)
{
    const int oldSize = count();
    mNonBMPCount = 0;
    mOneByteCount = 0;
    mTwoByteCount = 0;
    mThreeByteCount = 0;
    mFourByteCount = 0;
    if (mMode == CharCount)
    {
        mCodePoints.reserve(mLength.maxChars);
    }
    else if (mMode == ByteCount)
    {
        mCodePoints.reserve(mLength.maxBytes / 1.5); //just a guess, assuming 1.5 bytes per char
    }

    if (oldSize != 0)
    {
        emit mParent->topLevelDataInformation()->_childrenAboutToBeRemoved(mParent, 0, oldSize);
        emit mParent->topLevelDataInformation()->_childrenRemoved(mParent, 0, oldSize);
    }

    const uint oldMax = mCodePoints.size();
    quint64 remaining = bitsRemaining;
    Okteta::Address addr = address;
    uint count = 0;
    mEofReached = false;
    if (((mMode & CharCount) && mLength.maxChars == 0)
            || ((mMode & ByteCount) && mLength.maxBytes < 2))
        return 0;

    bool eofAtStart = false;
    if (bitsRemaining < 8)
        eofAtStart = true;

    while (true)
    {
        if (remaining < 8)
        {
            mEofReached = true;
            break;
        }
        uint codePoint;
        quint8 byte = input->byte(addr);
        bool terminate = false;

        if (byte <= ASCII_MAX)
        {
            mOneByteCount++;
            codePoint = byte;
        }
        else if ((byte & 0xe0) == 0xc0)
        {
            //two byte sequence
            if (byte == 0xc0 || byte == 0xc1)
            {
                mOneByteCount++;
                mErrorIndices[count] = 1;
                codePoint = byte;
            }
            else if (remaining < 16)
            {
                mOneByteCount++;
                mEofReached = true;
                mErrorIndices[count] = 1;
                codePoint = byte;
            }
            mTwoByteCount++;
            remaining -= 8;
            addr++;
            quint8 byte2 = input->byte(addr);
            if ((byte2 & 0xc0) != 0x80)
            {
                mErrorIndices[count] = 2;
                codePoint = (byte << 8) | byte2; //just put the raw bytes in case of error
            }
            codePoint = (byte2 & 0x3f) | ((byte & 0x1f) << 6);
        }
        else if ((byte & 0xf0) == 0xe0)
        {
            if (remaining < 16)
            {
                mEofReached = true;
                mErrorIndices[count] = 1;
                codePoint = byte;
                mOneByteCount++;
            }
            else if (remaining < 24)
            {
                mEofReached = true;
                mErrorIndices[count] = 2;
                remaining -= 8;
                addr++;
                codePoint = (byte << 8) | input->byte(addr);
                mTwoByteCount++;
            }
            else {
                mThreeByteCount++;
                remaining -= 16;
                addr++;
                quint8 byte2 = input->byte(addr);
                addr++;
                quint8 byte3 = input->byte(addr);
                if ((byte2 & 0xc0) != 0x80 || (byte3 & 0xc0) != 0x80)
                {
                    mErrorIndices[count] = 3;
                    codePoint = (byte << 16) | (byte2 << 8) | byte3; //just put the raw bytes in case of error
                }
                else
                {
                    codePoint = (byte3 & 0x3f) | ((byte2 & 0x3f) << 6) | ((byte & 0x1f) << 12);
                }
            }
        }
        else if ((byte & 0xf8) == 0xf0)
        {
            if (remaining < 16)
            {
                mEofReached = true;
                mErrorIndices[count] = 1;
                codePoint = byte;
                mOneByteCount++;
            }
            else if (remaining < 24)
            {
                mEofReached = true;
                mErrorIndices[count] = 2;
                addr++;
                remaining -= 8;
                codePoint = (byte << 8) | input->byte(addr);
                mTwoByteCount++;
            }
            else if (remaining < 32)
            {
                mEofReached = true;
                mErrorIndices[count] = 3;
                codePoint = (byte << 16) | (input->byte(addr + 1) << 8) | input->byte(addr + 2);
                addr += 2;
                remaining -= 16;
                mThreeByteCount++;
            }
            else
            {
                mFourByteCount++;
                remaining -= 24;
                addr++;
                quint8 byte2 = input->byte(addr);
                addr++;
                quint8 byte3 = input->byte(addr);
                addr++;
                quint8 byte4 = input->byte(addr);
                if ((byte2 & 0xc0) != 0x80 || (byte3 & 0xc0) != 0x80 || (byte4 & 0xc0) != 0x80)
                {
                    mErrorIndices[count] = 3;
                    codePoint = (byte << 16) | (byte2 << 8) | byte3; //just put the raw bytes in case of error
                }
                else
                {
                    codePoint = (byte4 & 0x3f) | ((byte3 & 0x3f) << 6)
                        | ((byte2 & 0x3f) << 12) | ((byte & 0x1f) << 18);
                    if (codePoint > UNICODE_MAX)
                    {
                        mErrorIndices[count] = 4;
                        //just put the raw bytes in case of error
                        codePoint = (byte << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
                    }
                }
            }
        }
        else {
            mErrorIndices[count] = 1;
            codePoint = byte;
            mOneByteCount++;
        }

        if (codePoint > BMP_MAX)
            mNonBMPCount++;
        if (count < oldMax)
            mCodePoints[count] = codePoint;
        else
            mCodePoints.append(codePoint);

        remaining -= 8;
        addr++;
        count++;

        //now check if we have to terminate
        if (mMode & Sequence)
        {
            if (codePoint == mTerminationCodePoint)
                terminate = true;
        }
        if (mMode & ByteCount)
        {
            // divide by two in case someone set length to an odd number of bytes
            if ((addr - address) >= mLength.maxBytes)
                terminate = true;
        }
        if (mMode & CharCount)
        {
            if (count >= mLength.maxChars)
                terminate = true;
        }
        if (mMode == None) {
            kDebug() << "no termination mode set!!";
            Q_ASSERT(false);
        }
        if (terminate)
            break;
    }
    mCodePoints.resize(count);
    emit mParent->topLevelDataInformation()->_childrenAboutToBeInserted(mParent, 0, count);
    emit mParent->topLevelDataInformation()->_childrenInserted(mParent, 0, count);

    if (eofAtStart)
        return -1;
    return (addr - address) * 8;
}

BitCount32 Utf8StringData::size() const
{
    //add 16 for every non BMP char, since they use 32 bits
    return (mOneByteCount + mTwoByteCount * 2 + mThreeByteCount * 3 + mFourByteCount * 4) * 8;
}

BitCount32 Utf8StringData::sizeAt(int i) const
{
    Q_ASSERT(i >= 0 && i <= count());
    quint8 isError = mErrorIndices[i];
    if (isError)
        return isError * 8; //error is number of bytes
    uint val = mCodePoints.at(i);
    if (val < 0x80)
        return 8;
    else if (val < 0x7ff)
        return 16;
    else if (val < 0xffff)
        return 24;
    else
        return 32;
}

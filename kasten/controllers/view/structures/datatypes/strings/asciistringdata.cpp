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


#include "asciistringdata.h"
#include "stringdatainformation.h"
#include "../topleveldatainformation.h"

#include <abstractbytearraymodel.h>

#include <KLocale>
#include <QVarLengthArray>

AsciiStringData::AsciiStringData(StringDataInformation* parent): StringData(parent)
{
}

AsciiStringData::~AsciiStringData()
{
}

qint64 AsciiStringData::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
{
    const int oldSize = count();
    if (mMode == CharCount || mMode == ByteCount) //same for ascii
    {
        mData.reserve(mLength.maxChars);
    }
    if (oldSize != 0)
    {
        emit mParent->topLevelDataInformation()->_childrenAboutToBeRemoved(mParent, 0, oldSize);
        emit mParent->topLevelDataInformation()->_childrenRemoved(mParent, 0, oldSize);
    }


    quint64 remaining = bitsRemaining;
    Okteta::Address addr = address;
    int count = 0;
    mEofReached = false;
    const int oldMax = mData.size();
    if (((mMode & CharCount) && mLength.maxChars == 0) || ((mMode & ByteCount) && mLength.maxBytes == 0))
        return 0; //nothing to read

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
        uchar val = input->byte(addr);
        bool terminate = false;

        if (count < oldMax)
            mData[count] = val;
        else
            mData.append(val);

        remaining -= 8;
        addr++;
        count++;

        //now check if we have to terminate
        if (mMode & Sequence)
        {
            if ((quint32(val) & 0xff) == mTerminationCodePoint)
                terminate = true;
        }
        if ((mMode & CharCount)  || (mMode & ByteCount))
        {
            if ((unsigned)count >= mLength.maxChars)
                terminate = true;
        }
        if (mMode == None) {
            kDebug() << "no termination mode set!!";
            Q_ASSERT(false);
        }
        if (terminate)
            break;
    }
    mData.resize(count);
    emit mParent->topLevelDataInformation()->_childrenAboutToBeInserted(mParent, 0, count);
    emit mParent->topLevelDataInformation()->_childrenInserted(mParent, 0, count);

    if (eofAtStart)
        return -1;
    return (addr - address) * 8;
}

BitCount32 AsciiStringData::sizeAt(int i) const
{
    Q_ASSERT(i >= 0 && i < mData.size());
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
        if (val > ASCII_MAX)
        {
            if (skipInvalid) {
                max--;
                i--;
                continue;
            }
            else
            {
                buf[i] = QChar::ReplacementCharacter;
            }
        }
        else
        {
            buf[i] = QChar::fromAscii(val);
        }
    }
    return QString(buf.constData(), max);
}

QString AsciiStringData::stringValue(int row) const
{
    Q_ASSERT(row >= 0 && row < mData.size());
    uchar val = mData.at(row);
    if (val > ASCII_MAX)
        return i18n("Non-ASCII char: 0x%1", val);
    else
        return QChar::fromAscii(val);
}

QString AsciiStringData::charType() const
{
    return i18n("ASCII char");
}

int AsciiStringData::count() const
{
    return mData.size();
}

QString AsciiStringData::typeName() const
{
    return i18n("ASCII string");
}

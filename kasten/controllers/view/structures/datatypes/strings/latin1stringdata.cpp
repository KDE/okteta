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


#include "latin1stringdata.h"
#include "../topleveldatainformation.h"
#include "stringdatainformation.h"

#include <abstractbytearraymodel.h>

#include <KLocale>
#include <QVarLengthArray>

Latin1StringData::Latin1StringData(StringDataInformation* parent): StringData(parent)
{
}

Latin1StringData::~Latin1StringData()
{
}

qint64 Latin1StringData::read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining)
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

BitCount32 Latin1StringData::sizeAt(int i) const
{
    Q_ASSERT(i >= 0 && i < mData.size());
    return 8;
}

BitCount32 Latin1StringData::size() const
{
    return mData.size() * 8;
}

QString Latin1StringData::completeString(bool skipInvalid) const
{
    Q_UNUSED(skipInvalid) // all are valid
    int max = mData.size();
    QVarLengthArray<QChar> buf(max);
    for (int i = 0; i < max; ++i) {
        uchar val = mData.at(i);
        buf[i] = QChar::fromLatin1(val);
    }
    return QString(buf.constData(), max);
}

QString Latin1StringData::stringValue(int row) const
{
    Q_ASSERT(row >= 0 && row < mData.size());
    uchar val = mData.at(row);
    return QChar::fromLatin1(val);
}

QString Latin1StringData::charType() const
{
    return i18n("Latin1 char");
}

int Latin1StringData::count() const
{
    return mData.size();
}

QString Latin1StringData::typeName() const
{
    return i18n("Latin1 string");
}

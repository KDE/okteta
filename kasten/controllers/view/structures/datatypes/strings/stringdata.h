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


#ifndef STRINGDATA_H
#define STRINGDATA_H

#include <QtCore/QString>

#include <address.h>
#include "../datainformationbase.h"

class StringDataInformation;

namespace Okteta {
    class AbstractByteArrayModel;
}

class StringData
{
private:
    Q_DISABLE_COPY(StringData)
public:
    enum TerminationMode {
        None = 0x0,
        Sequence = 0x1, //termination sequence (i.e. '\0')
        CharCount = 0x2, //numer of unicode code points
        ByteCount = 0x4, //maximum number of bytes
        SeqOrCharCount = CharCount + Sequence, //whatever comes first
        SeqOrByteCount = ByteCount + Sequence //whatever comes first
    };
    Q_DECLARE_FLAGS(TerminationModes, TerminationMode)

    explicit StringData(StringDataInformation* parent);
    virtual ~StringData();

    virtual QString typeName() const = 0;
    virtual int count() const = 0;
    virtual QString charType() const = 0;
    virtual QString stringValue(int row) const = 0;
    virtual QString completeString(bool skipInvalid = false) const = 0;
    virtual BitCount32 size() const = 0;
    virtual BitCount32 sizeAt(int i) const = 0;
    virtual qint64 read(Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) = 0;
    /** by default just sets value, if more logic is needed can be overridden */
    virtual void setLittleEndian(bool littleEndian);
    uint terminationMode() const;
    void setTerminationMode(uint mode);

    bool wasEof() const;

    uint maxByteCount() const;
    void setMaxByteCount(uint count);
    uint maxCharCount() const;
    void setMaxCharCount(uint count);
    uint terminationCodePoint() const;
    void setTerminationCodePoint(quint32 term);

    void copyTerminationFrom(const StringData* data);

    static const uint UNICODE_MAX = 0x10ffff;
    static const uint BMP_MAX = 0xffff;
    static const char ASCII_MAX = 0x7f;
protected:
    StringDataInformation* mParent;
    union {
        uint maxChars;
        uint maxBytes;
    } mLength;
    quint32 mTerminationCodePoint;
    int mMode : 4;
    bool mLittleEndian : 1;
    bool mEofReached : 1;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(StringData::TerminationModes)


inline uint StringData::terminationCodePoint() const
{
    return mTerminationCodePoint;
}

inline void StringData::setTerminationCodePoint(uint term)
{
    mTerminationCodePoint = term;
    mMode |= Sequence;
}

inline uint StringData::maxByteCount() const
{
    return mLength.maxBytes;
}

inline void StringData::setMaxByteCount(uint count)
{
    mLength.maxBytes = count;
    mMode |= ByteCount;
    mMode &= ~CharCount;
}

inline uint StringData::maxCharCount() const
{
    return mLength.maxChars;
}

inline void StringData::setMaxCharCount(uint count)
{
    mLength.maxChars = count;
    mMode |= CharCount;
    mMode &= ~ByteCount;
}

inline void StringData::copyTerminationFrom(const StringData* data)
{
    mMode = data->mMode;
    mTerminationCodePoint = data->mTerminationCodePoint;
    mLength = data->mLength;
}

inline uint StringData::terminationMode() const
{
    return mMode;
}

inline void StringData::setTerminationMode(uint mode)
{
    mMode = mode;
}

inline bool StringData::wasEof() const
{
    return mEofReached;
}


#endif // STRINGDATA_H

/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGDATA_HPP
#define KASTEN_STRINGDATA_HPP

#include <Okteta/Address>
#include "../datainformationbase.hpp"
// Qt
#include <QSysInfo>

class StringDataInformation;

namespace Okteta {
class AbstractByteArrayModel;
}

class StringData
{
public:
    enum TerminationMode
    {
        None = 0x0,
        Sequence = 0x1, // termination sequence (i.e. '\0')
        CharCount = 0x2, // numer of unicode code points
        ByteCount = 0x4, // maximum number of bytes
        SeqOrCharCount = CharCount + Sequence, // whatever comes first
        SeqOrByteCount = ByteCount + Sequence // whatever comes first
    };
    Q_DECLARE_FLAGS(TerminationModes, TerminationMode)

    explicit StringData(StringDataInformation* parent);
    StringData(const StringData&) = delete;

    virtual ~StringData();

    StringData& operator=(const StringData&) = delete;

public:
    virtual QString typeName() const = 0;
    virtual uint count() const = 0;
    virtual QString charType() const = 0;
    virtual QString stringValue(int row) const = 0;
    virtual QString completeString(bool skipInvalid = false) const = 0;
    virtual BitCount32 size() const = 0;
    virtual BitCount32 sizeAt(uint index) const = 0;
    virtual qint64 read(const Okteta::AbstractByteArrayModel* input, Okteta::Address address, BitCount64 bitsRemaining) = 0;
    /** by default just sets value, if more logic is needed can be overridden */
    virtual void setEndianness(QSysInfo::Endian endianness);
    TerminationMode terminationMode() const;
    void setTerminationMode(TerminationMode mode);

    bool wasEof() const;

    uint maxByteCount() const;
    void setMaxByteCount(uint count);
    uint maxCharCount() const;
    void setMaxCharCount(uint count);
    uint terminationCodePoint() const;
    void setTerminationCodePoint(quint32 term);

    void copyTerminationFrom(const StringData* data);

    static constexpr uint UNICODE_MAX = 0x10ffff;
    static constexpr uint BMP_MAX = 0xffff;
    static constexpr char ASCII_MAX = 0x7f;

protected:
    StringDataInformation* mParent;
    union {
        uint maxChars;
        uint maxBytes;
    } mLength;
    quint32 mTerminationCodePoint = 0;
    uint mMode : 4;
    bool mLittleEndian : 1;
    bool mEofReached : 1;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(StringData::TerminationModes)

inline uint StringData::terminationCodePoint() const
{
    return mTerminationCodePoint;
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

inline StringData::TerminationMode StringData::terminationMode() const
{
    return static_cast<TerminationMode>(mMode);
}

inline void StringData::setTerminationMode(StringData::TerminationMode mode)
{
    mMode = mode;
}

inline bool StringData::wasEof() const
{
    return mEofReached;
}

#endif // KASTEN_STRINGDATA_HPP

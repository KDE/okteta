/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraymodel.hpp"
#include "abstractbytearraymodel_p.hpp"

// lib
#include "charcodec.hpp"
#include "character.hpp"

namespace Okteta {

static constexpr int SearchedByteCountSignalLimit = 10000;

AbstractByteArrayModel::AbstractByteArrayModel(AbstractByteArrayModelPrivate* dd, QObject* parent)
    : QObject(parent)
    , d_ptr(dd)
{}

AbstractByteArrayModel::AbstractByteArrayModel(QObject* parent)
    : QObject(parent)
    , d_ptr(new AbstractByteArrayModelPrivate(this))
{}

AbstractByteArrayModel::~AbstractByteArrayModel() = default;

bool AbstractByteArrayModel::isReadOnly() const { return false; }

void AbstractByteArrayModel::setReadOnly(bool isReadOnly)
{
    Q_UNUSED(isReadOnly)
}

Size AbstractByteArrayModel::insert(Address offset, const Byte* insertData, int insertLength)
{
    return replace(offset, 0, insertData, insertLength);
}

Size AbstractByteArrayModel::remove(const AddressRange& removeRange)
{
    replace(removeRange, nullptr, 0);
    return removeRange.width(); // TODO: check if this is true
}

Size AbstractByteArrayModel::copyTo(Byte* dest, const AddressRange& _copyRange) const
{
    AddressRange copyRange(_copyRange);
    copyRange.restrictEndTo(size() - 1);

    const Address copyRangeEnd = copyRange.end();
    for (Address i = copyRange.start(); i <= copyRangeEnd; ++i) {
        *dest++ = byte(i);
    }

    return copyRange.width();
}

Address AbstractByteArrayModel::indexOf(const Byte* pattern, int patternLength, Address fromOffset, Address toOffset) const
{
    Address result = -1;

    const Address lastOffset = size() - 1;
    if ((toOffset < 0) || lastOffset < toOffset) {
        toOffset = lastOffset;
    }

    const Address lastFrom = toOffset - patternLength + 1;
    Size nextSignalByteCount = fromOffset + SearchedByteCountSignalLimit;

    for (Address i = fromOffset; i <= lastFrom; ++i) {
        int c = 0;
        for (; c < patternLength; ++c) {
            if (pattern[c] != byte(i + c)) {
                break;
            }
        }

        if (nextSignalByteCount <= i) {
            nextSignalByteCount += SearchedByteCountSignalLimit;
            emit searchedBytes(i - fromOffset + 1);
        }

        if (c == patternLength) {
            result = i;
            break;
        }
    }

    return result;
}

Address AbstractByteArrayModel::lastIndexOf(const Byte* pattern, int patternLength, Address fromOffset, Address toOffset) const
{
    Address result = -1;

    const Address lastFrom = size() - patternLength;

    if (fromOffset < 0) {
        fromOffset = lastFrom + 1 + fromOffset;
    } else if (fromOffset > lastFrom) {
        fromOffset = lastFrom;
    }

    if (toOffset < 0) {
        toOffset = 0;
    }

    Size nextSignalByteCount = fromOffset - SearchedByteCountSignalLimit;

    for (Address i = fromOffset; i >= toOffset; --i) {
        int c = 0;
        for (; c < patternLength; ++c) {
            if (pattern[c] != byte(i + c)) {
                break;
            }
        }

        if (nextSignalByteCount >= i) {
            nextSignalByteCount -= SearchedByteCountSignalLimit;
            emit searchedBytes(i - fromOffset + 1);
        }

        if (c == patternLength) {
            result = i;
            break;
        }
    }

    return result;
}

static QByteArray toLower(const QByteArray& _pattern, const CharCodec* charCodec)
{
    QByteArray result(_pattern);

    const int patternLength = result.size();
    char* pattern = result.data();
    for (int i = 0; i < patternLength; ++i) {
        const Character decodedChar = charCodec->decode(pattern[i]);

        if (decodedChar.isUndefined()) {
            continue;
        }

        charCodec->encode(reinterpret_cast<Byte*>(&pattern[i]), decodedChar.toLower());
    }

    return result;
}

Address AbstractByteArrayModel::indexOfCaseInsensitive(const CharCodec* charCodec, const QByteArray& _pattern, Address fromOffset, Address toOffset) const
{
    Address result = -1;

    const QByteArray lowerPattern = toLower(_pattern, charCodec);
    const Byte* const pattern = reinterpret_cast<const Byte*>(lowerPattern.constData());
    const int patternLength = lowerPattern.size();
    const Address lastOffset = size() - 1;
    if ((toOffset < 0) || lastOffset < toOffset) {
        toOffset = lastOffset;
    }
    const Address lastFrom = toOffset - patternLength + 1;

    Address nextSignalByteCount = fromOffset + SearchedByteCountSignalLimit;

    for (Address i = fromOffset; i <= lastFrom; ++i) {
        int c = 0;
        for (; c < patternLength; ++c) {
            Byte _byte = byte(i + c);

            // turn to lowercase if possible
            // TODO: optimize, like caching and not reencoding chars without a lower letter
            const Okteta::Character decodedChar = charCodec->decode(_byte);
            if (!decodedChar.isUndefined()) {
                charCodec->encode(&_byte, decodedChar.toLower());
            }

            if (_byte != pattern[c]) {
                break;
            }
        }

        if (nextSignalByteCount <= i) {
            nextSignalByteCount += SearchedByteCountSignalLimit;
            emit searchedBytes(i - fromOffset + 1);
        }

        if (c == patternLength) {
            result = i;
            break;
        }
    }

    return result;
}

Address AbstractByteArrayModel::lastIndexOfCaseInsensitive(const CharCodec* charCodec, const QByteArray& _pattern, Address fromOffset, Address toOffset) const
{
    Address result = -1;

    const QByteArray lowerPattern = toLower(_pattern, charCodec);
    const Byte* const pattern = reinterpret_cast<const Byte*>(lowerPattern.constData());
    const int patternLength = lowerPattern.size();
    const Address lastFrom = size() - patternLength;

    if (fromOffset < 0) {
        fromOffset = lastFrom + 1 + fromOffset;
    } else if (fromOffset > lastFrom) {
        fromOffset = lastFrom;
    }

    if (toOffset < 0) {
        toOffset = 0;
    }

    Address nextSignalByteCount = fromOffset - SearchedByteCountSignalLimit;

    for (Address i = fromOffset; i >= toOffset; --i) {
        int c = 0;
        for (; c < patternLength; ++c) {
            Byte _byte = byte(i + c);

            // turn to lowercase if possible
            // TODO: optimize, like caching and not reencoding chars without a lower letter
            const Okteta::Character decodedChar = charCodec->decode(_byte);
            if (!decodedChar.isUndefined()) {
                charCodec->encode(&_byte, decodedChar.toLower());
            }

            if (_byte != pattern[c]) {
                break;
            }
        }

        if (nextSignalByteCount >= i) {
            nextSignalByteCount -= SearchedByteCountSignalLimit;
            emit searchedBytes(i - fromOffset + 1);
        }

        if (c == patternLength) {
            result = i;
            break;
        }
    }

    return result;
}

}

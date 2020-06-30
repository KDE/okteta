/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2005, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "textbytearrayanalyzer.hpp"

// lib
#include "abstractbytearraymodel.hpp"
#include "character.hpp"
#include "charcodec.hpp"

namespace Okteta {

class TextByteArrayAnalyzerPrivate
{
public:
    const AbstractByteArrayModel* const byteArrayModel;
    const CharCodec* const charCodec;

public:
    TextByteArrayAnalyzerPrivate(const AbstractByteArrayModel* byteArrayModel, const CharCodec* charCodec);
};

TextByteArrayAnalyzerPrivate::TextByteArrayAnalyzerPrivate(const AbstractByteArrayModel* byteArrayModel, const CharCodec* charCodec)
    : byteArrayModel(byteArrayModel)
    , charCodec(charCodec)
{}


TextByteArrayAnalyzer::TextByteArrayAnalyzer(const AbstractByteArrayModel* byteArrayModel, const CharCodec* charCodec)
    : d_ptr(new TextByteArrayAnalyzerPrivate(byteArrayModel, charCodec))
{
}

TextByteArrayAnalyzer::~TextByteArrayAnalyzer() = default;

AddressRange TextByteArrayAnalyzer::wordSection(Address index) const
{
    return isWordChar(index) ?
           AddressRange(indexOfWordStart(index), indexOfWordEnd(index)) :
           AddressRange();
}

bool TextByteArrayAnalyzer::isWordChar(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    const Character decodedChar = d->charCodec->decode(d->byteArrayModel->byte(index));
    return !decodedChar.isUndefined() && decodedChar.isLetterOrNumber();
}

Address TextByteArrayAnalyzer::indexOfPreviousWordStart(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    const Size size = d->byteArrayModel->size();
    // already at the start or can the result only be 0?
    if (index == 0 || size < 3) {
        return 0;
    }

    // search in two rounds: first for the next char, than for the next nonchar
    // after that return the index of the one before
    bool lookingForFirstWordChar = false;
    for (; index > 0; --index) {
        if (!isWordChar(index - 1)) {
            if (!lookingForFirstWordChar) {
                continue;
            }
            return(index);
        }
        if (!lookingForFirstWordChar) {
            lookingForFirstWordChar = true;
        }
    }

    return 0;
}

Address TextByteArrayAnalyzer::indexOfNextWordStart(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    const Size size = d->byteArrayModel->size();
    bool lookingForFirstWordChar = false;
    for (; index < size; ++index) {
        if (isWordChar(index)) {
            if (!lookingForFirstWordChar) {
                continue;
            }
            return index;
        }
        if (!lookingForFirstWordChar) {
            lookingForFirstWordChar = true;
        }
    }

    // if no more word found, go to the end
    return size;
}

Address TextByteArrayAnalyzer::indexOfBeforeNextWordStart(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    const Size size = d->byteArrayModel->size();
    bool lookingForFirstWordChar = false;
    for (; index < size; ++index) {
        if (isWordChar(index)) {
            if (!lookingForFirstWordChar) {
                continue;
            }
            return index - 1;
        }
        if (!lookingForFirstWordChar) {
            lookingForFirstWordChar = true;
        }
    }

    // if no more word found, go to the end
    return size - 1;
}

Address TextByteArrayAnalyzer::indexOfWordStart(Address index) const
{
    for (; index > 0; --index) {
        if (!isWordChar(index - 1)) {
            return(index);
        }
    }

    return 0;
}

Address TextByteArrayAnalyzer::indexOfWordEnd(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    const Size size = d->byteArrayModel->size();
    for (++index; index < size; ++index) {
        if (!isWordChar(index)) {
            return index - 1;
        }
    }

    // word reaches the end
    return size - 1;
}

Address TextByteArrayAnalyzer::indexOfLeftWordSelect(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    // word at index?
    if (isWordChar(index)) {
        // search for word start to the left
        for (; index > 0; --index) {
            if (!isWordChar(index - 1)) {
                return index;
            }
        }

        // reached start, so return it
        return 0;
    }

    const Size size = d->byteArrayModel->size();
    // search for word start to the right
    for (++index; index < size; ++index) {
        if (isWordChar(index)) {
            return index;
        }
    }

    // word reaches the end, so step behind
    return size;
}

Address TextByteArrayAnalyzer::indexOfRightWordSelect(Address index) const
{
    Q_D(const TextByteArrayAnalyzer);

    // TODO: should this check be here or with the caller?
    // the later would need another function to search the previous word end
    const Size size = d->byteArrayModel->size();
    bool searchToLeft;
    if (index >= size) {
        index = size;
        searchToLeft = true;
    } else {
        searchToLeft = !isWordChar(index);
    }
    // no word at index?
    if (searchToLeft) {
        // search for word end to the left
        for (; index > 0; --index) {
            if (isWordChar(index - 1)) {
                return index;
            }
        }

        // reached start, so return it
        return 0;
    }

    for (++index; index < size; ++index) {
        // search for word end to the right
        if (!isWordChar(index)) {
            return index;
        }
    }

    // word reaches the end, so step behind
    return size;
}

/*
Address TextByteArrayAnalyzer::indexOfBehindWordEnd( Address index ) const
{
   // no word at index?
   return !::isWordChar(byte(index)) ? indexOfBehindLeftWordEnd(index) : indexOfBehindRightWordEnd(index+1)
}


Address TextByteArrayAnalyzer::indexOfBehindRightWordEnd( Address index ) const
{
   for( ; index<size(); ++index )
   {
        if( !::isWordChar(byte(index)) )
            return index;
   }
   // word reaches the end, so step behind
   return size();
}


Address TextByteArrayAnalyzer::indexOfBehindLeftWordEnd( Address index ) const
{
   for( --index; index>=0; --index )
   {
        if( ::isWordChar(byte(index)) )
            return index+1;
   }
   // word reaches the end, so step behind
   return 0;
}
*/

QString TextByteArrayAnalyzer::text(Address index, Address lastIndex) const
{
    Q_D(const TextByteArrayAnalyzer);

    QString result;

    const Address lastValidIndex = d->byteArrayModel->size() - 1;
    const Address behindLastIndex =
        ((lastIndex <0 || lastIndex> lastValidIndex) ? lastValidIndex : lastIndex) + 1;

    const Size maxTextLength = behindLastIndex - index;
    result.reserve(maxTextLength);

    for (; index < behindLastIndex; ++index) {
        const Character decodedChar = d->charCodec->decode(d->byteArrayModel->byte(index));
        // TODO: handle line breaks, separators and spacing, controlled by flags given as parameter
        const bool isTextChar = (!decodedChar.isUndefined() &&
                                 (decodedChar.isLetterOrNumber() || decodedChar.isSpace() || decodedChar.isPunct()));

        if (!isTextChar) {
            break;
        }

        result.append(decodedChar);
    }

    result.squeeze();

    return result;
}

}

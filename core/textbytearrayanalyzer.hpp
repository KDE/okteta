/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2005, 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_TEXTBYTEARRAYANALYZER_HPP
#define OKTETA_TEXTBYTEARRAYANALYZER_HPP

// lib
#include "oktetacore_export.hpp"
#include "addressrange.hpp"
// Std
#include <memory>

class QString;

namespace Okteta {

class AbstractByteArrayModel;
class CharCodec;

/**
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETACORE_EXPORT TextByteArrayAnalyzer
{
public:
    TextByteArrayAnalyzer(const AbstractByteArrayModel* byteArrayModel, const CharCodec* charCodec);
    TextByteArrayAnalyzer(const TextByteArrayAnalyzer&) = delete;
    TextByteArrayAnalyzer(TextByteArrayAnalyzer&&) = delete;

    ~TextByteArrayAnalyzer();

    TextByteArrayAnalyzer& operator=(const TextByteArrayAnalyzer&) = delete;
    TextByteArrayAnalyzer& operator=(TextByteArrayAnalyzer&&) = delete;

public:
    /** searches for the start of the word including the given index.
     * if no other nonwordchar preceds this is 0;
     * If the byte at the given index is already a nonword char the given index is returned.
     * @param index index to start with
     * @return index of the first char of the current word or the given index if there is none
     */
    [[nodiscard]]
    Address indexOfWordStart(Address index) const;
    [[nodiscard]]
    Address indexOfLeftWordSelect(Address index) const;
    /** searches for the end of the word including the given index.
     * If the byte at the given index is already a nonword char the given index is returned.
     * if no other nonwordchar follows, that of the last byte;
     * @param index index to start with
     * @return index of the last char of the current word or the given index if there is none
     */
    [[nodiscard]]
    Address indexOfWordEnd(Address index) const;
    /** searches for the first char after the end of the word including the given index.
     * If the byte at the given index is already a nonword char the given index is returned.
     * if no other nonwordchar follows that of behind the last byte;
     * @param index index to start with
     * @return index of the first char after the current word or the given index if there is none
     */
    [[nodiscard]]
    Address indexOfRightWordSelect(Address index) const;
    /** searches for the first char after the end of the word including the given index.
     * If the byte at the given index is already a nonword char the given index is returned.
     * if no other nonwordchar follows that of behind the last byte;
     * @param index index to start with
     * @return index of the first char after the current word or the given index if there is none
     */
//    Address indexOfBehindLeftWordEnd( Address index ) const;
    /** searches for the first char after the end of the word including the given index.
     * If the byte at the given index is already a nonword char the given index is returned.
     * if no other nonwordchar follows that of behind the last byte;
     * @param index index to start with
     * @return index of the first char after the current word or the given index if there is none
     */
//    Address indexOfBehindRightWordEnd( Address index ) const;
    /** searches the start of the next previous word that does not include the given index,
     * if no further word is found 0 is returned.
     * if the index is out of range the behaviour is undefined.
     * @param index
     * @return index of the next previous word start or 0
     */
    [[nodiscard]]
    Address indexOfPreviousWordStart(Address index) const;
    /** searches for the start of the next word not including the given index.
     * if there isn't a next word the index behind end is returned
     * @param index
     * @return index of the start of the next word or behind end
     */
    [[nodiscard]]
    Address indexOfNextWordStart(Address index) const;
    /** searches for the start of the next word not including the given index.
     * if there isn't a next word the index of the end is returned
     * @param index index to start with
     * @return index of the last nonword char before the next word or the last index
     */
    [[nodiscard]]
    Address indexOfBeforeNextWordStart(Address index) const;

    /** if index is out of range the behaviour is undefined
     * @param index
     * @return @c true if the byte at position i is a char of type CharType
     */
    [[nodiscard]]
    bool isWordChar(Address index) const;

    /** returns the section with a word around index.
     * if there is no word the section is empty
     * @param index
     * @return the section with a word around index.
     */
    [[nodiscard]]
    AddressRange wordSection(Address index) const;

    /** returns the text starting at the given index until the first non-text byte
     * if there is no text byte at the index the result is empty.
     * @param index
     * @param lastIndex if -1 lastIndex is set to the end of th byte array.
     * @return the text starting at the index
     */
    [[nodiscard]]
    QString text(Address index, Address lastIndex = -1) const;

private:
    const std::unique_ptr<class TextByteArrayAnalyzerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(TextByteArrayAnalyzer)
};

}

#endif

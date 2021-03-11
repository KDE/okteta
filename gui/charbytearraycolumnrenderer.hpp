/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHARBYTEARRAYCOLUMNRENDERER_HPP
#define OKTETA_CHARBYTEARRAYCOLUMNRENDERER_HPP

// lib
#include "abstractbytearraycolumnrenderer.hpp"

namespace Okteta {

class CharByteArrayColumnRendererPrivate;

/** buffer column that interprets the bytes as chars
 *
 * @author Friedrich W. H. Kossebau
 */
class OKTETAGUI_EXPORT CharByteArrayColumnRenderer : public AbstractByteArrayColumnRenderer
{
public:
    CharByteArrayColumnRenderer(AbstractColumnStylist* stylist,
                                AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges);
    ~CharByteArrayColumnRenderer() override;

public: // modification access
    /** sets whether "unprintable" chars (>32) should be displayed in the char column
     * with their corresponding character.
     * @param showingNonprinting
     * returns true if there was a change
     */
    bool setShowingNonprinting(bool showingNonprinting = true);
    /** sets the substitute character for "unprintable" chars
     * returns true if there was a change
     */
    bool setSubstituteChar(QChar substituteChar);
    /** sets the undefined character for "undefined" chars
     * returns true if there was a change
     */
    bool setUndefinedChar(QChar undefinedChar);

public: // value access
    /** returns true if "unprintable" chars (>32) are displayed in the char column
     * with their corresponding character, default is false
     */
    bool isShowingNonprinting() const;
    /** returns the actually used substitute character for "unprintable" chars, default is '.' */
    QChar substituteChar() const;
    /** returns the actually used undefined character for "undefined" chars, default is '?' */
    QChar undefinedChar() const;

private:
    Q_DECLARE_PRIVATE(CharByteArrayColumnRenderer)
};

}

#endif

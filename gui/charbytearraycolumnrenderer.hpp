/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_CHARBYTEARRAYCOLUMNRENDERER_HPP
#define OKTETA_CHARBYTEARRAYCOLUMNRENDERER_HPP

// lib
#include <okteta/abstractbytearraycolumnrenderer.hpp>

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

/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charbytearraycolumnrenderer_p.hpp"

// Qt
#include <QPainter>

namespace Okteta {

void CharByteArrayColumnRendererPrivate::renderByteText(QPainter* painter, Byte byte, Character byteChar, const QColor& color) const
{
    Q_UNUSED(byte)

    // turn into a drawable String
    const QString text(byteChar.isUndefined() ?                       Okteta::Character(mUndefinedChar) :
                       !(mShowingNonprinting || byteChar.isPrint()) ? Okteta::Character(mSubstituteChar) :
                                                                      byteChar);

    painter->setPen(color);
    painter->drawText(0, mDigitBaseLine, text);
}

}

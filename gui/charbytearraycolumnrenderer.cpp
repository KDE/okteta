/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "charbytearraycolumnrenderer.hpp"
#include "charbytearraycolumnrenderer_p.hpp"

namespace Okteta {

CharByteArrayColumnRenderer::CharByteArrayColumnRenderer(AbstractColumnStylist* stylist,
                                                         AbstractByteArrayModel* byteArrayModel, ByteArrayTableLayout* layout, ByteArrayTableRanges* ranges)
    : AbstractByteArrayColumnRenderer(new CharByteArrayColumnRendererPrivate(this, stylist, byteArrayModel, layout, ranges))
{
    Q_D(CharByteArrayColumnRenderer);

    d->init();
}

CharByteArrayColumnRenderer::~CharByteArrayColumnRenderer() = default;

bool CharByteArrayColumnRenderer::isShowingNonprinting() const
{
    Q_D(const CharByteArrayColumnRenderer);

    return d->isShowingNonprinting();
}

QChar CharByteArrayColumnRenderer::substituteChar() const
{
    Q_D(const CharByteArrayColumnRenderer);

    return d->substituteChar();
}

QChar CharByteArrayColumnRenderer::undefinedChar() const
{
    Q_D(const CharByteArrayColumnRenderer);

    return d->undefinedChar();
}

bool CharByteArrayColumnRenderer::setSubstituteChar(QChar substituteChar)
{
    Q_D(CharByteArrayColumnRenderer);

    return d->setSubstituteChar(substituteChar);
}

bool CharByteArrayColumnRenderer::setUndefinedChar(QChar undefinedChar)
{
    Q_D(CharByteArrayColumnRenderer);

    return d->setUndefinedChar(undefinedChar);
}

bool CharByteArrayColumnRenderer::setShowingNonprinting(bool showingNonprinting)
{
    Q_D(CharByteArrayColumnRenderer);

    return d->setShowingNonprinting(showingNonprinting);
}

}

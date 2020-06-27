/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARBYTEARRAYCOLUMNTEXTRENDERER_HPP
#define KASTEN_CHARBYTEARRAYCOLUMNTEXTRENDERER_HPP

// lib
#include "abstractbytearraycolumntextrenderer.hpp"
// Qt
#include <QChar>

namespace Okteta {
class CharCodec;
}

namespace Kasten {

class CharByteArrayColumnTextRenderer : public AbstractByteArrayColumnTextRenderer
{
public:
    CharByteArrayColumnTextRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                    const Okteta::CoordRange& coordRange,
                                    int noOfBytesPerLine, int byteSpacingWidth, int noOfGroupedBytes,
                                    const QString& charCodecName, QChar substituteChar, QChar undefinedChar);

    ~CharByteArrayColumnTextRenderer() override;

protected: // AbstractByteArrayColumnTextRenderer API
    void renderLine(QTextStream* stream, bool isSubline) const override;

private:
    const Okteta::CharCodec* mCharCodec;
    const QChar mSubstituteChar;
    const QChar mUndefinedChar;
};

}

#endif

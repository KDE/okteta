/*
    SPDX-FileCopyrightText: 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARBYTEARRAYCOLUMNHTMLRENDERER_HPP
#define KASTEN_CHARBYTEARRAYCOLUMNHTMLRENDERER_HPP

// lib
#include "abstractbytearraycolumnhtmlrenderer.hpp"
// Qt
#include <QChar>
// Std
#include <memory>

namespace Okteta {
class CharCodec;
}

namespace Kasten {

class CharByteArrayColumnHtmlRenderer : public AbstractByteArrayColumnHtmlRenderer
{
public:
    CharByteArrayColumnHtmlRenderer(const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                    const Okteta::CoordRange& coordRange,
                                    int noOfBytesPerLine,
                                    const QString& charCodecName, QChar substituteChar, QChar undefinedChar);

    ~CharByteArrayColumnHtmlRenderer() override;

protected: // AbstractByteArrayColumnHtmlRenderer API
    void renderLine(QTextStream* stream, bool isSubline) const override;

private:
    const std::unique_ptr<const Okteta::CharCodec> mCharCodec;
    const QChar mSubstituteChar;
    const QChar mUndefinedChar;
};

}

#endif

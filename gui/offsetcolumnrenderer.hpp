/*
    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_OFFSETCOLUMNRENDERER_HPP
#define OKTETA_OFFSETCOLUMNRENDERER_HPP

// lib
#include "offsetformat.hpp"
#include "abstractcolumnrenderer.hpp"
#include "line.hpp"

class QFontMetrics;

namespace Okteta {

class ByteArrayTableLayout;
class OffsetColumnRendererPrivate;

/**
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT OffsetColumnRenderer : public AbstractColumnRenderer
{
public:
    OffsetColumnRenderer(AbstractColumnStylist* stylist, ByteArrayTableLayout* layout, OffsetFormat::Format format);

    ~OffsetColumnRenderer() override;

public:    // AbstractColumnRenderer API
    void renderFirstLine(QPainter* painter, PixelXRange Xs, Line firstLineIndex) override;
    void renderNextLine(QPainter* painter) override;
    void renderColumn(QPainter* painter, PixelXRange Xs, PixelYRange Ys) override;
    void renderEmptyColumn(QPainter* painter, PixelXRange Xs, PixelYRange Ys) override;

public:
    void setFormat(OffsetFormat::Format format, const QFontMetrics& fontMetrics);
    void setFontMetrics(const QFontMetrics& fontMetrics);
    void setMargin(PixelX margin);

public: // read access
//     int delta() const;
    [[nodiscard]]
    OffsetFormat::Format format() const;

private:
    Q_DECLARE_PRIVATE(OffsetColumnRenderer)
};

}

#endif

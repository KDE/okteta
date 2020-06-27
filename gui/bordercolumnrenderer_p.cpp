/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bordercolumnrenderer_p.hpp"

// lib
#include <abstractcolumnstylist.hpp>
// Qt
#include <QPainter>
#include <QStyle>

namespace Okteta {

static constexpr PixelX BorderMargin = 4;
static constexpr PixelX LineWidth = 1;
static constexpr PixelX BorderWidth = 2 * BorderMargin + LineWidth;
static constexpr PixelX LineX = BorderMargin;

void BorderColumnRendererPrivate::init()
{
    Q_Q(BorderColumnRenderer);

    q->setWidth(m_lineDrawn ? BorderWidth : BorderMargin);
}

void BorderColumnRendererPrivate::renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_Q(BorderColumnRenderer);

    q->AbstractColumnRenderer::renderColumn(painter, Xs, Ys);

    renderBorderLine(painter, Xs, Ys);
}

void BorderColumnRendererPrivate::renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_Q(BorderColumnRenderer);

    q->AbstractColumnRenderer::renderEmptyColumn(painter, Xs, Ys);

    if (m_inEmpty) {
        renderBorderLine(painter, Xs, Ys);
    }
}

void BorderColumnRendererPrivate::renderBorderLine(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_Q(BorderColumnRenderer);

    const PixelX viewGlobalLineX = q->x() + LineX;

    if (m_lineDrawn && Xs.includes(viewGlobalLineX)) {
        const int lineColor = -1; // TODO: viewport->style()->styleHint( QStyle::SH_Table_GridLineColor, 0, viewport );

        painter->setPen(lineColor != -1 ? (QRgb)lineColor : mStylist->palette().mid().color());
        painter->drawLine(viewGlobalLineX, Ys.start(), viewGlobalLineX, Ys.end());
    }
}

}

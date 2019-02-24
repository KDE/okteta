/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

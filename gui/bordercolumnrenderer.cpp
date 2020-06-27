/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bordercolumnrenderer.hpp"
#include "bordercolumnrenderer_p.hpp"


namespace Okteta {

BorderColumnRenderer::BorderColumnRenderer(AbstractColumnStylist* stylist, bool lineDrawn, bool inEmpty)
    : AbstractColumnRenderer(new BorderColumnRendererPrivate(this, stylist, lineDrawn, inEmpty))
{
    Q_D(BorderColumnRenderer);

    d->init();
}

BorderColumnRenderer::~BorderColumnRenderer() = default;

void BorderColumnRenderer::renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_D(BorderColumnRenderer);

    d->renderColumn(painter, Xs, Ys);
}

void BorderColumnRenderer::renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys)
{
    Q_D(BorderColumnRenderer);

    d->renderEmptyColumn(painter, Xs, Ys);
}

}

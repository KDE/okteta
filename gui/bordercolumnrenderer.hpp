/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BORDERCOLUMNRENDERER_HPP
#define OKTETA_BORDERCOLUMNRENDERER_HPP

// lib
#include "abstractcolumnrenderer.hpp"

namespace Okteta {

class BorderColumnRendererPrivate;

/** column that does nothing but draw a vertical line in the middle of the column
 *
 * @author Friedrich W. H. Kossebau
 */

class OKTETAGUI_EXPORT BorderColumnRenderer : public AbstractColumnRenderer
{
public:
    BorderColumnRenderer(AbstractColumnStylist* stylist, bool lineDrawn, bool inEmpty = true);
    ~BorderColumnRenderer() override;

public: // AbstractColumnRenderer-API
    void renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys) override;
    void renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys) override;

private:
    Q_DECLARE_PRIVATE(BorderColumnRenderer)
};

}

#endif

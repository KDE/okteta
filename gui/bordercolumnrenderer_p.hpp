/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_BORDERCOLUMNRENDERER_P_HPP
#define OKTETA_BORDERCOLUMNRENDERER_P_HPP

// lib
#include "bordercolumnrenderer.hpp"
#include "abstractcolumnrenderer_p.hpp"

namespace Okteta {

class BorderColumnRendererPrivate : public AbstractColumnRendererPrivate
{
public:
    BorderColumnRendererPrivate(BorderColumnRenderer* q, AbstractColumnStylist* stylist,
                                bool lineDrawn, bool inEmpty);
    ~BorderColumnRendererPrivate() override;

public:
    void init();

public: // AbstractColumnRenderer-API
    void renderColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);
    void renderEmptyColumn(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

private:
    void renderBorderLine(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

private:
    /** true if we are between two columns and should show a line */
    bool m_lineDrawn : 1;
    /** true if the line is also drawn in empty space */
    bool m_inEmpty : 1;

private:
    BorderColumnRenderer* const q_ptr;
    Q_DECLARE_PUBLIC(BorderColumnRenderer)
};


inline BorderColumnRendererPrivate::BorderColumnRendererPrivate(BorderColumnRenderer* q,
                                                                AbstractColumnStylist* stylist,
                                                                bool lineDrawn, bool inEmpty)
    : AbstractColumnRendererPrivate(stylist)
    , m_lineDrawn(lineDrawn)
    , m_inEmpty(inEmpty)
    , q_ptr(q)
{
}

inline BorderColumnRendererPrivate::~BorderColumnRendererPrivate() = default;

}

#endif

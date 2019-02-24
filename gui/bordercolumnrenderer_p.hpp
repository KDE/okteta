/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

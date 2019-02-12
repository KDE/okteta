/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_BORDERCOLUMNRENDERER_HPP
#define OKTETA_BORDERCOLUMNRENDERER_HPP

// lib
#include <okteta/abstractcolumnrenderer.hpp>

namespace Okteta {

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
    void renderBorderLine(QPainter* painter, const PixelXRange& Xs, const PixelYRange& Ys);

private:
    /** true if we are between two columns and should show a line */
    bool mLineDrawn;
    /** true if the line is also drawn in empty space */
    bool mInEmpty;
};

}

#endif

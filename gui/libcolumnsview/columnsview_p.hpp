/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2003,2007,2008,2019 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETA_COLUMNSVIEW_P_HPP
#define OKTETA_COLUMNSVIEW_P_HPP

// lib
#include "columnsview.hpp"
#include "abstractcolumnrenderer.hpp"
// Qt
#include <QVector>

namespace Okteta {

class ColumnsViewPrivate
{
public:
    explicit ColumnsViewPrivate(/*bool R,*/ ColumnsView* parent);
    ColumnsViewPrivate() = delete;
    virtual ~ColumnsViewPrivate();

public:
    void init();
    void updateWidths();

public: // calculated
    /** collection of all the columns. All columns will be autodeleted. */
    QVector<AbstractColumnRenderer*> columns;
    /** the number of lines which the column view has */
    LineSize NoOfLines = 0;
    /** the height of each line in pixels */
    PixelY LineHeight = 0;
    /** the width of all visible columns together */
    PixelX ColumnsWidth = 0;

public:
//    bool Reversed;

protected:
    ColumnsView* const q_ptr;

private:
    Q_DECLARE_PUBLIC(ColumnsView)
};


inline ColumnsViewPrivate::ColumnsViewPrivate(/*bool R,*/ ColumnsView* parent)
    : q_ptr(parent)
//    Reversed( R )
{
}

inline ColumnsViewPrivate::~ColumnsViewPrivate()
{
    qDeleteAll(columns);
}

inline void ColumnsViewPrivate::updateWidths()
{
    ColumnsWidth = 0;
    for (auto column : qAsConst(columns)) {
        column->setX(ColumnsWidth);
        ColumnsWidth += column->visibleWidth();
    }
}

}

#endif

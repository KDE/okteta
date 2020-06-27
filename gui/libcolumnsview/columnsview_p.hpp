/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2003, 2007, 2008, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

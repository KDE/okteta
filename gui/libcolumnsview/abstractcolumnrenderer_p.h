/*
    This file is part of the Okteta Gui library, part of the KDE project.

    Copyright 2003,2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KHE_UI_ABSTRACTCOLUMNRENDERER_P_H
#define KHE_UI_ABSTRACTCOLUMNRENDERER_P_H

#include "abstractcolumnrenderer.h"

// lib
#include "columnsview.h"
// Qt
#include <QtGui/QPainter>


namespace KHEUI
{

class ColumnRendererPrivate
{
  public:
    explicit ColumnRendererPrivate( ColumnsView* columnsView );

  public:
    void renderBlankLine( QPainter *painter ) const;
    void renderEmptyColumn( QPainter* painter, const KPixelXs& xSpan, const KPixelYs& ySpan );

  public: // general column data
    /** pointer to the view */
    ColumnsView* mColumnView;
    /** should Column be displayed? */
    bool mIsVisible;

    /** buffered value */
    KPixelY mLineHeight;

    /** span of the column in pixel */
    KPixelXs mXSpan;
};


inline ColumnRendererPrivate::ColumnRendererPrivate( ColumnsView* columnsView )
 : mColumnView( columnsView ),
   mIsVisible( true ),  //TODO: would false be better?
   mLineHeight( 0 ),//columnsView->lineHeight() ),
   mXSpan( KHE::Section::fromWidth(0,0) )
{
}

inline void ColumnRendererPrivate::renderBlankLine( QPainter* painter ) const
{
    if( mLineHeight > 0 )
    {
        const QWidget* viewport = mColumnView->viewport();
        painter->fillRect( 0,0, mXSpan.width(),mLineHeight,
                           viewport->palette().brush(viewport->backgroundRole()) );
    }
}

inline void ColumnRendererPrivate::renderEmptyColumn( QPainter* painter, const KPixelXs& _xSpan, const KPixelYs& ySpan )
{
    KPixelXs xSpan( _xSpan );
    xSpan.restrictTo( mXSpan );

    const QWidget* viewport = mColumnView->viewport();
    painter->fillRect( xSpan.start(),ySpan.start(), xSpan.width(),ySpan.width(),
                       viewport->palette().brush(viewport->backgroundRole()) );
}

}

#endif

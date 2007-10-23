/***************************************************************************
                          browserextension.cpp  -  description
                             -------------------
    begin                : Di Nov 16 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "browserextension.h"

// part
#include "part.h"
// hexedit gui
#include <kbytearrayview.h>
// KDE
#include <KAction>


OktetaBrowserExtension::OktetaBrowserExtension( OktetaPart *p )
 : KParts::BrowserExtension( p ),
   part( p )
{
    setObjectName( "oktetapartbrowserextension" );
    connect( part->view, SIGNAL( selectionChanged( bool ) ), SLOT( onSelectionChanged( bool ) ) );
}

void OktetaBrowserExtension::copy()
{
    part->view->copy();
}


void OktetaBrowserExtension::onSelectionChanged( bool HasSelection )
{
    emit enableAction( "copy", HasSelection );
}


void OktetaBrowserExtension::saveState( QDataStream &stream )
{
    KParts::BrowserExtension::saveState( stream );

    KHEUI::KByteArrayView *view = part->view;

    stream << (int)view->offsetColumnVisible() << view->visibleBufferColumns()
        << (int)view->resizeStyle() << (int)view->coding() 
        << view->encodingName() << (int)view->showUnprintable()
        << view->xOffset() << view->yOffset()
        << view->cursorPosition() << (int)view->isCursorBehind()
        << view->cursorColumn();
}


void OktetaBrowserExtension::restoreState( QDataStream &stream )
{
    KParts::BrowserExtension::restoreState( stream );

    int offsetColumnVisible;
    int visibleBufferColumns;
    int resizeStyle;
    int coding;
    QString encodingName;
    int showUnprintable;
    int x, y;
    int position;
    int cursorBehind;
    int cursorColumn;

    stream >> offsetColumnVisible >> visibleBufferColumns >> resizeStyle >> coding >> encodingName >> showUnprintable 
           >> x >> y >> position >> cursorBehind >> cursorColumn;

    KHEUI::KByteArrayView *view = part->view;

    view->toggleOffsetColumn( offsetColumnVisible );
    view->showBufferColumns( visibleBufferColumns );
    view->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)resizeStyle );
    view->setCoding( (KHEUI::KByteArrayView::KCoding)coding );
    view->setEncoding( encodingName );
    view->setShowUnprintable( showUnprintable );
    view->setColumnsPos( x, y );
    view->setCursorPosition( position, cursorBehind );
    view->setCursorColumn( (KHEUI::KByteArrayView::KDataColumnId)cursorColumn );

    part->fitActionSettings();
}

#include "browserextension.moc"

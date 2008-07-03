/*
    This file is part of the Okteta KPart module, part of the KDE project.

    Copyright 2004 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "browserextension.h"

// part
#include "part.h"
// Okteta gui
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
        << view->encodingName() << (int)view->showsNonprinting()
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
    int showsNonprinting;
    int x, y;
    int position;
    int cursorBehind;
    int cursorColumn;

    stream >> offsetColumnVisible >> visibleBufferColumns >> resizeStyle >> coding >> encodingName >> showsNonprinting 
           >> x >> y >> position >> cursorBehind >> cursorColumn;

    KHEUI::KByteArrayView *view = part->view;

    view->toggleOffsetColumn( offsetColumnVisible );
    view->setVisibleByteArrayColumns( visibleBufferColumns );
    view->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)resizeStyle );
    view->setCoding( (KHEUI::KByteArrayView::KCoding)coding );
    view->setEncoding( encodingName );
    view->setShowsNonprinting( showsNonprinting );
    view->setColumnsPos( x, y );
    view->setCursorPosition( position, cursorBehind );
    view->setCursorColumn( (KHEUI::KByteArrayView::ByteArrayColumnId)cursorColumn );

    part->fitActionSettings();
}

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
#include <bytearraycolumnview.h>
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

    KHEUI::ByteArrayColumnView *view = part->view;

    stream << (int)view->offsetColumnVisible() << view->visibleCodings()
        << (int)view->resizeStyle() << (int)view->valueCoding() 
        << view->charCodingName() << (int)view->showsNonprinting()
        << view->xOffset() << view->yOffset()
        << view->cursorPosition() << (int)view->isCursorBehind()
        << view->activeCoding();
}


void OktetaBrowserExtension::restoreState( QDataStream &stream )
{
    KParts::BrowserExtension::restoreState( stream );

    int offsetColumnVisible;
    int visibleCodings;
    int resizeStyle;
    int valueCoding;
    QString charCodingName;
    int showsNonprinting;
    int x, y;
    int position;
    int cursorBehind;
    int activeCoding;

    stream >> offsetColumnVisible >> visibleCodings >> resizeStyle >> valueCoding >> charCodingName >> showsNonprinting 
           >> x >> y >> position >> cursorBehind >> activeCoding;

    KHEUI::ByteArrayColumnView *view = part->view;

    view->toggleOffsetColumn( offsetColumnVisible );
    view->setVisibleCodings( visibleCodings );
    view->setResizeStyle( (KHEUI::ByteArrayColumnView::ResizeStyle)resizeStyle );
    view->setValueCoding( (KHEUI::ByteArrayColumnView::ValueCoding)valueCoding );
    view->setCharCoding( charCodingName );
    view->setShowsNonprinting( showsNonprinting );
    view->setColumnsPos( x, y );
    view->setCursorPosition( position, cursorBehind );
    view->setActiveCoding( (KHEUI::ByteArrayColumnView::CodingTypeId)activeCoding );

    part->fitActionSettings();
}

#include "browserextension.moc"

/*
    This file is part of the Okteta KPart module, made within the KDE community.

    Copyright 2004,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
// Okteta Kasten
#include <print/printcontroller.h>
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Qt
#include <QtGui/QClipboard>
#include <QtGui/QApplication>


OktetaBrowserExtension::OktetaBrowserExtension( OktetaPart* part )
  : KParts::BrowserExtension( part ),
    mPart( part )
{
    setObjectName( QLatin1String( "oktetapartbrowserextension" ) );

    connect( mPart, SIGNAL(hasSelectedDataChanged(bool)),
             SLOT(onSelectionChanged(bool)) );

    emit enableAction( "copy", false );
    emit enableAction( "print", true);
}

void OktetaBrowserExtension::copy()
{
    QMimeData* data = mPart->byteArrayView()->copySelectedData();
    if( !data )
      return;

    QApplication::clipboard()->setMimeData( data, QClipboard::Clipboard );
}

void OktetaBrowserExtension::print()
{
    mPart->printController()->print();
}


void OktetaBrowserExtension::onSelectionChanged( bool hasSelection )
{
    emit enableAction( "copy", hasSelection );
}


void OktetaBrowserExtension::saveState( QDataStream& stream )
{
    KParts::BrowserExtension::saveState( stream );

    Kasten2::ByteArrayView* view = mPart->byteArrayView();

    stream << (int)view->offsetColumnVisible() << view->visibleByteArrayCodings()
        << (int)view->layoutStyle() << (int)view->valueCoding() 
        << view->charCodingName() << (int)view->showsNonprinting()
//         << view->xOffset() << view->yOffset()
        << view->cursorPosition()
//         << (int)view->isCursorBehind()
//         << view->activeCoding()
        ;
}


void OktetaBrowserExtension::restoreState( QDataStream& stream )
{
    KParts::BrowserExtension::restoreState( stream );

    int offsetColumnVisible;
    int visibleCodings;
    int layoutStyle;
    int valueCoding;
    QString charCodingName;
    int showsNonprinting;
//     int x, y;
    int position;
//     int cursorBehind;
//     int activeCoding;

    stream >> offsetColumnVisible >> visibleCodings >> layoutStyle >> valueCoding >> charCodingName >> showsNonprinting
//            >> x >> y
           >> position
//            >> cursorBehind
//            >> activeCoding
           ;

    Kasten2::ByteArrayView* view = mPart->byteArrayView();

    view->toggleOffsetColumn( offsetColumnVisible );
    view->setVisibleByteArrayCodings( visibleCodings );
    view->setLayoutStyle( layoutStyle );
    view->setValueCoding( valueCoding );
    view->setCharCoding( charCodingName );
    view->setShowsNonprinting( showsNonprinting );
//     view->setColumnsPos( x, y );
    view->setCursorPosition( position );//, cursorBehind );
//     view->setActiveCoding( (Okteta::ByteArrayColumnView::CodingTypeId)activeCoding );
}

/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewconfigcontroller.h"

// lib
#include <kbytearraydisplay.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
// KDE
#include <KXMLGUIClient>
#include <KLocale>
#include <KActionCollection>
#include <KSelectAction>
#include <KToggleAction>


ViewConfigController::ViewConfigController( KXMLGUIClient* guiClient )
 : mByteArrayView( 0 )
{
    KActionCollection* actionCollection = guiClient->actionCollection();

    // value encoding
    mCodingAction = actionCollection->add<KSelectAction>( "view_valuecoding" );
    mCodingAction->setText( i18nc("@title:menu","&Value Coding") );
    QStringList list;
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format","&Hexadecimal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "&Decimal")     );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "&Octal")       );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "&Binary")      );
    mCodingAction->setItems( list );
    connect( mCodingAction, SIGNAL(triggered(int)), SLOT(setCoding(int)) );

    // document encoding
    mEncodingAction = actionCollection->add<KSelectAction>( "view_charencoding" );
    mEncodingAction->setText( i18nc("@title:menu","&Char Encoding") );
    mEncodingAction->setItems( KHECore::KCharCodec::codecNames() );
    connect( mEncodingAction, SIGNAL(triggered(int)), SLOT(setEncoding(int)) );

    mShowsNonprintingAction = actionCollection->add<KToggleAction>( "view_showsnonprinting" );
    mShowsNonprintingAction->setText( i18nc("@option:check","Show &Non-printing Chars") );
    connect( mShowsNonprintingAction, SIGNAL(triggered(bool)), SLOT(setShowsNonprinting(bool)) );

    // resize style
    mResizeStyleAction = actionCollection->add<KSelectAction>( "resizestyle" );
    mResizeStyleAction->setText( i18nc("@title:menu","&Resize Style") );
    list.clear();
    list.append( i18nc("@item:inmenu","&No Resize") );
    list.append( i18nc("@item:inmenu","&Lock Groups") );
    list.append( i18nc("@item:inmenu","&Full Size Usage") );
    mResizeStyleAction->setItems( list );
    connect( mResizeStyleAction, SIGNAL(triggered(int)), SLOT(setResizeStyle(int)) );

    mShowOffsetColumnAction = actionCollection->add<KToggleAction>( "view_lineoffset" );
    mShowOffsetColumnAction->setText( i18nc("@option:check","Show &Line Offset") );
    mShowOffsetColumnAction->setShortcut( Qt::Key_F11 );
    connect( mShowOffsetColumnAction, SIGNAL(triggered(bool)), SLOT(toggleOffsetColumn(bool)) );

    // show buffer columns
    mToggleColumnsAction = actionCollection->add<KSelectAction>( "togglecolumns" );
    mToggleColumnsAction->setText( i18nc("@title:menu","&Columns") );
    list.clear();
    list.append( i18nc("@item:inmenu","&Values Column") );
    list.append( i18nc("@item:inmenu","&Chars Column") );
    list.append( i18nc("@item:inmenu","&Both Columns") );
    mToggleColumnsAction->setItems( list );
    connect( mToggleColumnsAction, SIGNAL(triggered(int)), SLOT(toggleValueCharColumns(int)) );
    setTargetModel( 0 );
}

void ViewConfigController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    KByteArrayDisplay* view = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    if( mByteArrayView )
    {
        mShowOffsetColumnAction->setChecked( mByteArrayView->offsetColumnVisible() );
        mShowsNonprintingAction->setChecked( mByteArrayView->showsNonprinting() );

        mCodingAction->setCurrentItem( (int)mByteArrayView->coding() );
        mEncodingAction->setCurrentItem( KHECore::KCharCodec::codecNames().indexOf(mByteArrayView->encodingName()) );

        mResizeStyleAction->setCurrentItem( (int)mByteArrayView->resizeStyle() );

        mToggleColumnsAction->setCurrentItem( (int)mByteArrayView->visibleBufferColumns()-1 );
    }
    const bool hasView = ( mByteArrayView != 0 );
    mCodingAction->setEnabled( hasView );
    mEncodingAction->setEnabled( hasView );
    mShowsNonprintingAction->setEnabled( hasView );
    mResizeStyleAction->setEnabled( hasView );
    mShowOffsetColumnAction->setEnabled( hasView );
    mToggleColumnsAction->setEnabled( hasView );
}


void ViewConfigController::setCoding( int coding )
{
    mByteArrayView->setCoding( (KHEUI::KByteArrayView::KCoding)coding );
}

void ViewConfigController::setShowsNonprinting( bool on )
{
    mByteArrayView->setShowsNonprinting( on );
}

void ViewConfigController::toggleOffsetColumn( bool on )
{
    mByteArrayView->toggleOffsetColumn( on );
}

void ViewConfigController::setResizeStyle( int resizeStyle )
{
    mByteArrayView->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)resizeStyle );
}

void ViewConfigController::setEncoding( int encoding )
{
    mByteArrayView->setEncoding( KHECore::KCharCodec::codecNames()[encoding] );
}

void ViewConfigController::toggleValueCharColumns( int visibleColumns )
{
    mByteArrayView->setVisibleByteArrayColumns( visibleColumns+1 );
}

/***************************************************************************
                          viewconfigcontroller.cpp  -  description
                             -------------------
    begin                : Wed Nov 8 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "viewconfigcontroller.h"

// kakao
#include <kviewmanager.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
// KDE
#include <KXmlGuiWindow>
#include <KLocale>
#include <KActionCollection>
#include <KSelectAction>
#include <KToggleAction>


ViewConfigController::ViewConfigController( KXmlGuiWindow *MW )
 : MainWindow( MW ), HexEdit( 0 )
{
    KActionCollection *actionCollection = MainWindow->actionCollection();

    // value encoding
    CodingAction = actionCollection->add<KSelectAction>( "view_valuecoding" );
    CodingAction->setText( i18n("&Value Coding") );
    QStringList List;
    List.append( i18n("&Hexadecimal") );
    List.append( i18n("&Decimal")     );
    List.append( i18n("&Octal")       );
    List.append( i18n("&Binary")      );
    CodingAction->setItems( List );
    connect( CodingAction, SIGNAL(triggered(int)), SLOT(setCoding(int)) );

    // document encoding
    EncodingAction = actionCollection->add<KSelectAction>( "view_charencoding" );
    EncodingAction->setText( i18n("&Char Encoding") );
    EncodingAction->setItems( KHECore::KCharCodec::codecNames() );
    connect( EncodingAction, SIGNAL(triggered(int)), SLOT(setEncoding(int)) );

    ShowUnprintableAction = actionCollection->add<KToggleAction>( "view_showunprintable" );
    ShowUnprintableAction->setText( i18n("Show &Unprintable Chars (<32)") );
    connect( ShowUnprintableAction, SIGNAL(triggered(bool)), SLOT(setShowUnprintable(bool)) );

    // resize style
    ResizeStyleAction = actionCollection->add<KSelectAction>( "resizestyle" );
    ResizeStyleAction->setText( i18n("&Resize Style") );
    List.clear();
    List.append( i18n("&No Resize") );
    List.append( i18n("&Lock Groups") );
    List.append( i18n("&Full Size Usage") );
    ResizeStyleAction->setItems( List );
    connect( ResizeStyleAction, SIGNAL(triggered(int)), SLOT(setResizeStyle(int)) );

    ShowOffsetColumnAction = actionCollection->add<KToggleAction>( "view_lineoffset" );
    ShowOffsetColumnAction->setText( i18n("&Line Offset") );
    ShowOffsetColumnAction->setShortcut( Qt::Key_F11 );
    connect( ShowOffsetColumnAction, SIGNAL(triggered(bool)), SLOT(toggleOffsetColumn(bool)) );

    // show buffer columns
    ToggleColumnsAction = actionCollection->add<KSelectAction>( "togglecolumns" );
    ToggleColumnsAction->setText( i18n("&Columns") );
    List.clear();
    List.append( i18n("&Values Column") );
    List.append( i18n("&Chars Column") );
    List.append( i18n("&Both Columns") );
    ToggleColumnsAction->setItems( List );
    connect( ToggleColumnsAction, SIGNAL(triggered(int)), SLOT(toggleValueCharColumns(int)) );
    setView( 0 );
}

void ViewConfigController::setView( KAbstractView *View )
{
    disconnect( HexEdit );

    HexEdit = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;

    if( HexEdit )
    {
        ShowOffsetColumnAction->setChecked( HexEdit->offsetColumnVisible() );
        ShowUnprintableAction->setChecked( HexEdit->showUnprintable() );

        CodingAction->setCurrentItem( (int)HexEdit->coding() );
        EncodingAction->setCurrentItem( KHECore::KCharCodec::codecNames().indexOf(HexEdit->encodingName()) );

        ResizeStyleAction->setCurrentItem( (int)HexEdit->resizeStyle() );

        ToggleColumnsAction->setCurrentItem( (int)HexEdit->visibleBufferColumns()-1 );
    }
    const bool HasView = ( HexEdit != 0 );
    CodingAction->setEnabled( HasView );
    EncodingAction->setEnabled( HasView );
    ShowUnprintableAction->setEnabled( HasView );
    ResizeStyleAction->setEnabled( HasView );
    ShowOffsetColumnAction->setEnabled( HasView );
    ToggleColumnsAction->setEnabled( HasView );
}


void ViewConfigController::setCoding( int Coding )
{
  HexEdit->setCoding( (KHEUI::KByteArrayView::KCoding)Coding );
}

void ViewConfigController::setShowUnprintable( bool on )
{
  HexEdit->setShowUnprintable( on );
}

void ViewConfigController::toggleOffsetColumn( bool on )
{
  HexEdit->toggleOffsetColumn( on );
}

void ViewConfigController::setResizeStyle( int ResizeStyle )
{
  HexEdit->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)ResizeStyle );
}

void ViewConfigController::setEncoding( int Encoding )
{
  HexEdit->setEncoding( KHECore::KCharCodec::codecNames()[Encoding] );
}

void ViewConfigController::toggleValueCharColumns( int VisibleColumns)
{
  HexEdit->showBufferColumns( VisibleColumns+1 );
}

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

// Kakao gui
#include <kabstractview.h>
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
 : MainWindow( MW ), ViewWidget( 0 )
{
    KActionCollection *actionCollection = MainWindow->actionCollection();

    // value encoding
    CodingAction = actionCollection->add<KSelectAction>( "view_valuecoding" );
    CodingAction->setText( i18nc("@title:menu","&Value Coding") );
    QStringList List;
    List.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format","&Hexadecimal") );
    List.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "&Decimal")     );
    List.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "&Octal")       );
    List.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "&Binary")      );
    CodingAction->setItems( List );
    connect( CodingAction, SIGNAL(triggered(int)), SLOT(setCoding(int)) );

    // document encoding
    EncodingAction = actionCollection->add<KSelectAction>( "view_charencoding" );
    EncodingAction->setText( i18nc("@title:menu","&Char Encoding") );
    EncodingAction->setItems( KHECore::KCharCodec::codecNames() );
    connect( EncodingAction, SIGNAL(triggered(int)), SLOT(setEncoding(int)) );

    ShowsNonprintingAction = actionCollection->add<KToggleAction>( "view_showsnonprinting" );
    ShowsNonprintingAction->setText( i18nc("@option:check","Show &Non-printing Chars") );
    connect( ShowsNonprintingAction, SIGNAL(triggered(bool)), SLOT(setShowsNonprinting(bool)) );

    // resize style
    ResizeStyleAction = actionCollection->add<KSelectAction>( "resizestyle" );
    ResizeStyleAction->setText( i18nc("@title:menu","&Resize Style") );
    List.clear();
    List.append( i18nc("@item:inmenu","&No Resize") );
    List.append( i18nc("@item:inmenu","&Lock Groups") );
    List.append( i18nc("@item:inmenu","&Full Size Usage") );
    ResizeStyleAction->setItems( List );
    connect( ResizeStyleAction, SIGNAL(triggered(int)), SLOT(setResizeStyle(int)) );

    ShowOffsetColumnAction = actionCollection->add<KToggleAction>( "view_lineoffset" );
    ShowOffsetColumnAction->setText( i18nc("@option:check","Show &Line Offset") );
    ShowOffsetColumnAction->setShortcut( Qt::Key_F11 );
    connect( ShowOffsetColumnAction, SIGNAL(triggered(bool)), SLOT(toggleOffsetColumn(bool)) );

    // show buffer columns
    ToggleColumnsAction = actionCollection->add<KSelectAction>( "togglecolumns" );
    ToggleColumnsAction->setText( i18nc("@title:menu","&Columns") );
    List.clear();
    List.append( i18nc("@item:inmenu","&Values Column") );
    List.append( i18nc("@item:inmenu","&Chars Column") );
    List.append( i18nc("@item:inmenu","&Both Columns") );
    ToggleColumnsAction->setItems( List );
    connect( ToggleColumnsAction, SIGNAL(triggered(int)), SLOT(toggleValueCharColumns(int)) );
    setView( 0 );
}

void ViewConfigController::setView( KAbstractView *View )
{
    if( ViewWidget ) ViewWidget->disconnect( this );

    ViewWidget = View ? static_cast<KHEUI::KByteArrayView *>( View->widget() ) : 0;

    if( ViewWidget )
    {
        ShowOffsetColumnAction->setChecked( ViewWidget->offsetColumnVisible() );
        ShowsNonprintingAction->setChecked( ViewWidget->showsNonprinting() );

        CodingAction->setCurrentItem( (int)ViewWidget->coding() );
        EncodingAction->setCurrentItem( KHECore::KCharCodec::codecNames().indexOf(ViewWidget->encodingName()) );

        ResizeStyleAction->setCurrentItem( (int)ViewWidget->resizeStyle() );

        ToggleColumnsAction->setCurrentItem( (int)ViewWidget->visibleBufferColumns()-1 );
    }
    const bool HasView = ( ViewWidget != 0 );
    CodingAction->setEnabled( HasView );
    EncodingAction->setEnabled( HasView );
    ShowsNonprintingAction->setEnabled( HasView );
    ResizeStyleAction->setEnabled( HasView );
    ShowOffsetColumnAction->setEnabled( HasView );
    ToggleColumnsAction->setEnabled( HasView );
}


void ViewConfigController::setCoding( int Coding )
{
  ViewWidget->setCoding( (KHEUI::KByteArrayView::KCoding)Coding );
}

void ViewConfigController::setShowsNonprinting( bool on )
{
  ViewWidget->setShowsNonprinting( on );
}

void ViewConfigController::toggleOffsetColumn( bool on )
{
  ViewWidget->toggleOffsetColumn( on );
}

void ViewConfigController::setResizeStyle( int ResizeStyle )
{
  ViewWidget->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)ResizeStyle );
}

void ViewConfigController::setEncoding( int Encoding )
{
  ViewWidget->setEncoding( KHECore::KCharCodec::codecNames()[Encoding] );
}

void ViewConfigController::toggleValueCharColumns( int VisibleColumns)
{
  ViewWidget->setVisibleByteArrayColumns( VisibleColumns+1 );
}

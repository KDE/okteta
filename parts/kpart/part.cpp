/*
    This file is part of the Okteta KPart module, part of the KDE project.

    Copyright 2003,2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "part.h"

// part
#include "partfactory.h"
#include "browserextension.h"
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kcharcodec.h>
// KDE
#include <KLocale>
//#include <kcomponentdata.h>
#include <KAction>
#include <KActionCollection>
#include <KStandardAction>
#include <KSelectAction>
#include <KToggleAction>
//#include <kglobalsettings.h>


static const char RCFileName[] = "oktetapartui.rc";


OktetaPart::OktetaPart( QWidget *parentWidget,
                        QObject *parent,
                        bool browserViewWanted )
 : KParts::ReadOnlyPart( parent )
{
    setComponentData( OktetaPartFactory::componentData() );

    view = new KHEUI::KByteArrayView( &fileByteArray, parentWidget );
    view->setNoOfBytesPerLine( 16 );
    view->setBufferSpacing( 3, 4, 10 );
    view->setShowsNonprinting( false );

    setWidget( view );

    setupActions( browserViewWanted );

    connect( view, SIGNAL(selectionChanged( bool )), SLOT(onSelectionChanged( bool )) );
    if( copyAction )
    {
        connect( view, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)) );
        copyAction->setEnabled( false );
    }

    if( browserViewWanted )
        new OktetaBrowserExtension( this );
}


void OktetaPart::setupActions( bool browserViewWanted )
{
    KActionCollection *actions = actionCollection();

    // create our actions
    copyAction = browserViewWanted ? 0 : KStandardAction::copy( view, SLOT(copy()), actions );

    KStandardAction::selectAll( this, SLOT(onSelectAll()), actions );
    deselectAction = KStandardAction::deselect( this, SLOT(onUnselect()), actions );

    // value encoding
    codingAction = actions->add<KSelectAction>( "view_valuecoding" );
    codingAction->setText( i18nc("@title:menu","&Value Coding") );
    QStringList strings;
    strings.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format","&Hexadecimal") );
    strings.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "&Decimal")     );
    strings.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "&Octal")       );
    strings.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "&Binary")      );
    codingAction->setItems( strings );
    connect( codingAction, SIGNAL(triggered(int)), SLOT(onSetCoding(int)) );

    // document encoding
    encodingAction = actions->add<KSelectAction>( "view_charencoding" );
    encodingAction->setText( i18nc("@title:menu","&Char Encoding") );
    encodingAction->setItems( KHECore::KCharCodec::codecNames() );
    connect( encodingAction, SIGNAL(triggered(int)), SLOT(onSetEncoding(int)) );

    showNonprintingAction = actions->add<KToggleAction>( "view_showsnonprinting" );
    showNonprintingAction->setText( i18nc("@option:check","Show &Non-printing Chars") );
    connect( showNonprintingAction, SIGNAL(triggered(bool)), SLOT(onSetShowsNonprinting(bool)) );

    KStandardAction::zoomIn(  view, SLOT(zoomIn()),  actions );
    KStandardAction::zoomOut( view, SLOT(zoomOut()), actions );

    // resize style
    resizeStyleAction = actions->add<KSelectAction>( "resizestyle" );
    resizeStyleAction->setText( i18nc("@title:menu","&Resize Style") );
    strings.clear();
    strings.append( i18nc("@item:inmenu","&No Resize") );
    strings.append( i18nc("@item:inmenu","&Lock Groups") );
    strings.append( i18nc("@item:inmenu","&Full Size Usage") );
    resizeStyleAction->setItems( strings );
    connect( resizeStyleAction, SIGNAL(triggered(int)), SLOT(onSetResizeStyle(int)) );

    showOffsetColumnAction = actions->add<KToggleAction>( "view_lineoffset" );
    showOffsetColumnAction->setText( i18nc("@option:check","&Line Offset") );
    showOffsetColumnAction->setShortcut( QKeySequence( Qt::Key_F11 ) );
    connect( showOffsetColumnAction, SIGNAL(triggered(bool)), SLOT(onToggleOffsetColumn(bool)) );

    // show buffer columns
    toggleColumnsAction = actions->add<KSelectAction>( "togglecolumns" );
    toggleColumnsAction->setText( i18nc("@title:menu","&Columns") );
    strings.clear();
    strings.append( i18nc("@item:inmenu","&Values Column") );
    strings.append( i18nc("@item:inmenu","&Chars Column") );
    strings.append( i18nc("@item:inmenu","&Both Columns") );
    toggleColumnsAction->setItems( strings );
    connect( toggleColumnsAction, SIGNAL(triggered(int)), SLOT(onToggleValueCharColumns(int)) );

    fitActionSettings();

    setXMLFile( RCFileName );
}


void OktetaPart::fitActionSettings()
{
    deselectAction->setEnabled( view->hasSelectedData() );

    showOffsetColumnAction->setChecked( view->offsetColumnVisible() );
    showNonprintingAction->setChecked( view->showsNonprinting() );

    codingAction->setCurrentItem( (int)view->coding() );
    encodingAction->setCurrentItem( KHECore::KCharCodec::codecNames().indexOf(view->encodingName()) );

    resizeStyleAction->setCurrentItem( (int)view->resizeStyle() );

    toggleColumnsAction->setCurrentItem( (int)view->visibleBufferColumns()-1 );
}


bool OktetaPart::openFile()
{
    fileByteArray.open( localFilePath() );
    view->setByteArrayModel( &fileByteArray );
    view->setCursorPosition( 0 );
    view->selectAll( false );

    return true;
}



void OktetaPart::onSelectionChanged( bool hasSelection )
{
    if( copyAction ) copyAction->setEnabled( hasSelection );
    deselectAction->setEnabled( hasSelection );
}


void OktetaPart::onSelectAll()
{
    view->selectAll( true );
}

void OktetaPart::onUnselect()
{
    view->selectAll( false );
}

void OktetaPart::onSetCoding( int Coding )
{
    view->setCoding( (KHEUI::KByteArrayView::KCoding)Coding );
}

void OktetaPart::onSetShowsNonprinting( bool on )
{
    view->setShowsNonprinting( on );
}

void OktetaPart::onToggleOffsetColumn( bool on )
{
    view->toggleOffsetColumn( on );
}

void OktetaPart::onSetResizeStyle( int ResizeStyle )
{
    view->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)ResizeStyle );
}

void OktetaPart::onSetEncoding( int Encoding )
{
    view->setEncoding( KHECore::KCharCodec::codecNames()[Encoding] );
}

void OktetaPart::onToggleValueCharColumns( int VisibleColumns)
{
    view->setVisibleByteArrayColumns( VisibleColumns+1 );
}

OktetaPart::~OktetaPart() {}

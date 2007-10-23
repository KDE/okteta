/***************************************************************************
                          part.cpp  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#include "part.h"

// part
#include "partfactory.h"
#include "browserextension.h"
// hexedit gui
#include <kbytearrayview.h>
// hexedit core
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
    view->setShowUnprintable( false );

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
    codingAction->setText( i18n("&Value Coding") );
    QStringList strings;
    strings.append( i18n("&Hexadecimal") );
    strings.append( i18n("&Decimal")     );
    strings.append( i18n("&Octal")       );
    strings.append( i18n("&Binary")      );
    codingAction->setItems( strings );
    connect( codingAction, SIGNAL(triggered(int)), SLOT(onSetCoding(int)) );

    // document encoding
    encodingAction = actions->add<KSelectAction>( "view_charencoding" );
    encodingAction->setText( i18n("&Char Encoding") );
    encodingAction->setItems( KHECore::KCharCodec::codecNames() );
    connect( encodingAction, SIGNAL(triggered(int)), SLOT(onSetEncoding(int)) );

    showUnprintableAction = actions->add<KToggleAction>( "view_showunprintable" );
    showUnprintableAction->setText( i18n("Show &Unprintable Chars (<32)") );
    connect( showUnprintableAction, SIGNAL(activated(int)), SLOT(onSetShowUnprintable()) );

    KStandardAction::zoomIn(  view, SLOT(zoomIn()),  actions );
    KStandardAction::zoomOut( view, SLOT(zoomOut()), actions );

    // resize style
    resizeStyleAction = actions->add<KSelectAction>( "resizestyle" );
    resizeStyleAction->setText( i18n("&Resize Style") );
    strings.clear();
    strings.append( i18n("&No Resize") );
    strings.append( i18n("&Lock Groups") );
    strings.append( i18n("&Full Size Usage") );
    resizeStyleAction->setItems( strings );
    connect( resizeStyleAction, SIGNAL(triggered(int)), SLOT(onSetResizeStyle(int)) );

    showOffsetColumnAction = actions->add<KToggleAction>( "view_lineoffset" );
    showOffsetColumnAction->setText( i18n("&Line Offset") );
    showOffsetColumnAction->setShortcut( QKeySequence( Qt::Key_F11 ) );
    connect( showOffsetColumnAction, SIGNAL(activated(int)), SLOT(onToggleOffsetColumn()) );

    // show buffer columns
    toggleColumnsAction = actions->add<KSelectAction>( "togglecolumns" );
    toggleColumnsAction->setText( i18n("&Columns") );
    strings.clear();
    strings.append( i18n("&Values Column") );
    strings.append( i18n("&Chars Column") );
    strings.append( i18n("&Both Columns") );
    toggleColumnsAction->setItems( strings );
    connect( toggleColumnsAction, SIGNAL(triggered(int)), SLOT(onToggleValueCharColumns(int)) );

    fitActionSettings();

    setXMLFile( RCFileName );
}


void OktetaPart::fitActionSettings()
{
    deselectAction->setEnabled( view->hasSelectedData() );

    showOffsetColumnAction->setChecked( view->offsetColumnVisible() );
    showUnprintableAction->setChecked( view->showUnprintable() );

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

void OktetaPart::onSetShowUnprintable()
{
    view->setShowUnprintable( showUnprintableAction->isChecked() );
}

void OktetaPart::onToggleOffsetColumn()
{
    view->toggleOffsetColumn( showOffsetColumnAction->isChecked() );
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
    view->showBufferColumns( VisibleColumns+1 );
}

OktetaPart::~OktetaPart() {}


#include "part.moc"

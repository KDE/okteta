/***************************************************************************
                          khepart.cpp  -  description
                             -------------------
    begin                : Don Jun 19 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// kde specific
#include <klocale.h>
//#include <kcomponentdata.h>
#include <kaction.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kselectaction.h>
#include <ktoggleaction.h>
//#include <kglobalsettings.h>
// corelib specific
#include <kcharcodec.h>
// uilib specific
#include <kbytearrayview.h>
// part specific
#include "khepartfactory.h"
#include "khebrowserextension.h"
#include "khepart.h"


static const char RCFileName[] = "khexedit2partui.rc";

KHexEditPart::KHexEditPart( QWidget *ParentWidget,
                            QObject *Parent,
                            bool BrowserViewWanted )
 : KParts::ReadOnlyPart( Parent )
{
  setComponentData( KHexEditPartFactory::componentData() );

  HexEdit = new KHEUI::KByteArrayView( &Wrapping, ParentWidget );
  HexEdit->setNoOfBytesPerLine( 16 );
  HexEdit->setBufferSpacing( 3, 4, 10 );
  HexEdit->setShowUnprintable( false );

  // notify the part that this is our internal widget
  setWidget( HexEdit );

  setupActions( BrowserViewWanted );

  connect( HexEdit, SIGNAL(selectionChanged( bool )), SLOT(onSelectionChanged( bool )) );
  if( CopyAction )
  {
    connect( HexEdit, SIGNAL(copyAvailable(bool)), CopyAction, SLOT(setEnabled(bool)) );
    CopyAction->setEnabled( false );
  }

  // plugin to browsers
  if( BrowserViewWanted )
    new KHexEditBrowserExtension( this );
}


void KHexEditPart::setupActions( bool BrowserViewWanted )
{
  KActionCollection *AC = actionCollection();
  // create our actions
  CopyAction = BrowserViewWanted ? 0 : KStandardAction::copy( HexEdit, SLOT(copy()), AC );

  KStandardAction::selectAll( this, SLOT(onSelectAll()), AC );
  DeselectAction = KStandardAction::deselect( this, SLOT(onUnselect()), AC );

  // value encoding
  CodingAction = AC->add<KSelectAction>( "view_valuecoding" );
  CodingAction->setText( i18n("&Value Coding") );
  QStringList List;
  List.append( i18n("&Hexadecimal") );
  List.append( i18n("&Decimal")     );
  List.append( i18n("&Octal")       );
  List.append( i18n("&Binary")      );
  CodingAction->setItems( List );
  connect( CodingAction, SIGNAL(triggered(int)), SLOT(onSetCoding(int)) );

  // document encoding
  EncodingAction = AC->add<KSelectAction>( "view_charencoding" );
  EncodingAction->setText( i18n("&Char Encoding") );
  EncodingAction->setItems( KHECore::KCharCodec::codecNames() );
  connect( EncodingAction, SIGNAL(triggered(int)), SLOT(onSetEncoding(int)) );

  ShowUnprintableAction = AC->add<KToggleAction>( "view_showunprintable" );
  ShowUnprintableAction->setText( i18n("Show &Unprintable Chars (<32)") );
  connect( ShowUnprintableAction, SIGNAL(activated(int)), SLOT(onSetShowUnprintable()) );

  KStandardAction::zoomIn(  HexEdit, SLOT(zoomIn()),  AC );
  KStandardAction::zoomOut( HexEdit, SLOT(zoomOut()), AC );

  // resize style
  ResizeStyleAction = AC->add<KSelectAction>( "resizestyle" );
  ResizeStyleAction->setText( i18n("&Resize Style") );
  List.clear();
  List.append( i18n("&No Resize") );
  List.append( i18n("&Lock Groups") );
  List.append( i18n("&Full Size Usage") );
  ResizeStyleAction->setItems( List );
  connect( ResizeStyleAction, SIGNAL(triggered(int)), SLOT(onSetResizeStyle(int)) );

  ShowOffsetColumnAction = AC->add<KToggleAction>( "view_lineoffset" );
  ShowOffsetColumnAction->setText( i18n("&Line Offset") );
  ShowOffsetColumnAction->setShortcut( QKeySequence( Qt::Key_F11 ) );
  connect( ShowOffsetColumnAction, SIGNAL(activated(int)), SLOT(onToggleOffsetColumn()) );

  // show buffer columns
  ToggleColumnsAction = AC->add<KSelectAction>( "togglecolumns" );
  ToggleColumnsAction->setText( i18n("&Columns") );
  List.clear();
  List.append( i18n("&Values Column") );
  List.append( i18n("&Chars Column") );
  List.append( i18n("&Both Columns") );
  ToggleColumnsAction->setItems( List );
  connect( ToggleColumnsAction, SIGNAL(triggered(int)), SLOT(onToggleValueCharColumns(int)) );

  fitActionSettings();

  // set our XML-UI resource file
  setXMLFile( RCFileName );
}


void KHexEditPart::fitActionSettings()
{
  DeselectAction->setEnabled( HexEdit->hasSelectedData() );

  ShowOffsetColumnAction->setChecked( HexEdit->offsetColumnVisible() );
  ShowUnprintableAction->setChecked( HexEdit->showUnprintable() );

  CodingAction->setCurrentItem( (int)HexEdit->coding() );
  EncodingAction->setCurrentItem( KHECore::KCharCodec::codecNames().indexOf(HexEdit->encodingName()) );

  ResizeStyleAction->setCurrentItem( (int)HexEdit->resizeStyle() );

  ToggleColumnsAction->setCurrentItem( (int)HexEdit->visibleBufferColumns()-1 );
}


bool KHexEditPart::openFile()
{
  Wrapping.open( m_file );
  HexEdit->setByteArrayModel( &Wrapping );
  HexEdit->setCursorPosition( 0 );
  HexEdit->selectAll( false );

  return true;
}



void KHexEditPart::onSelectionChanged( bool HasSelection )
{
  if( CopyAction ) CopyAction->setEnabled( HasSelection );
  DeselectAction->setEnabled( HasSelection );
}


void KHexEditPart::onSelectAll()
{
  HexEdit->selectAll( true );
}

void KHexEditPart::onUnselect()
{
  HexEdit->selectAll( false );
}

void KHexEditPart::onSetCoding( int Coding )
{
  HexEdit->setCoding( (KHEUI::KByteArrayView::KCoding)Coding );
}

void KHexEditPart::onSetShowUnprintable()
{
  HexEdit->setShowUnprintable( ShowUnprintableAction->isChecked() );
}

void KHexEditPart::onToggleOffsetColumn()
{
  HexEdit->toggleOffsetColumn( ShowOffsetColumnAction->isChecked() );
}

void KHexEditPart::onSetResizeStyle( int ResizeStyle )
{
  HexEdit->setResizeStyle( (KHEUI::KByteArrayView::KResizeStyle)ResizeStyle );
}

void KHexEditPart::onSetEncoding( int Encoding )
{
  HexEdit->setEncoding( KHECore::KCharCodec::codecNames()[Encoding] );
}

void KHexEditPart::onToggleValueCharColumns( int VisibleColumns)
{
  HexEdit->showBufferColumns( VisibleColumns+1 );
}

KHexEditPart::~KHexEditPart()
{
}


#include "khepart.moc"

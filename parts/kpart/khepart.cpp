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
//#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
//#include <kglobalsettings.h>
// app specific
#include "khexedit.h"
#include "kcharcodec.h"
#include "khepartfactory.h"
#include "khebrowserextension.h"
#include "khepart.h"

using namespace KHE;

static const char RCFileName[] = "khexedit2partui.rc";

KHexEditPart::KHexEditPart( QWidget *ParentWidget, const char *WidgetName,
                            QObject *Parent, const char *Name,
                            bool BrowserViewWanted )
 : KParts::ReadOnlyPart( Parent, Name )
{
  setInstance( KHexEditPartFactory::instance() );

  HexEdit = new KHexEdit( &Wrapping, ParentWidget, WidgetName );
  HexEdit->setNoOfBytesPerLine( 16 );
  HexEdit->setBufferSpacing( 3, 4, 10 );
  HexEdit->setShowUnprintable( false );

  // notify the part that this is our internal widget
  setWidget( HexEdit );

  setupActions( BrowserViewWanted );

  if( CopyAction )
  {
    connect( HexEdit, SIGNAL(copyAvailable(bool)), CopyAction,SLOT(setEnabled(bool)) );
    connect( HexEdit, SIGNAL(selectionChanged()),  this,      SLOT(slotSelectionChanged()) );
    CopyAction->setEnabled( false );
  }

  // plugin to browsers
  if( BrowserViewWanted )
    new KHexEditBrowserExtension( this );
}


KHexEditPart::~KHexEditPart()
{
}

/*
void KHexEditPart::setupTools( bool BrowserViewWanted )
{
  if( !BrowserViewWanted ) new KClipboardTool( this );
  
  new KZoomToolet( this );
  new KSelectToolet( this );
  new KHEValueCodingToolet( this );
  new KHECharEncodingToolet( this );
  new KHEResizeStyleToolet( this );
  new KHEColumnToggleToolet( this );
}
*/
void KHexEditPart::setupActions( bool BrowserViewWanted )
{
  KActionCollection *AC = actionCollection();
  // create our actions
  CopyAction = BrowserViewWanted ? 0 : KStdAction::copy( HexEdit, SLOT(copy()), AC );

  KStdAction::selectAll( this, SLOT(slotSelectAll()), AC );
  KStdAction::deselect(  this, SLOT(slotUnselect()),  AC );

  // value encoding
  CodingAction = new KSelectAction( i18n("&Value Coding"), 0, AC, "view_valuecoding" );
  QStringList List;
  List.append( i18n("&Hexadecimal") );
  List.append( i18n("&Decimal")     );
  List.append( i18n("&Octal")       );
  List.append( i18n("&Binary")      );
  CodingAction->setItems( List );
  connect( CodingAction, SIGNAL(activated(int)), this, SLOT(slotSetCoding(int)) );

  // document encoding
  EncodingAction = new KSelectAction( i18n("&Char Encoding"), 0, AC, "view_charencoding" );
  EncodingAction->setItems( KCharCodec::codecNames() );
  connect( EncodingAction, SIGNAL(activated(int)), this, SLOT(slotSetEncoding(int)) );

  ShowUnprintableAction = new KToggleAction( i18n("Show &Unprintable Chars (<32)"), 0, this, SLOT(slotSetShowUnprintable()), actionCollection(), "view_showunprintable" );

  KStdAction::zoomIn(  HexEdit, SLOT(zoomIn()),   actionCollection() );
  KStdAction::zoomOut( HexEdit, SLOT(zoomOut()),  actionCollection() );

  // resize style
  ResizeStyleAction = new KSelectAction( i18n("&Resize Style"), 0, AC, "resizestyle" );
  List.clear();
  List.append( i18n("&No Resize") );
  List.append( i18n("&Lock Groups") );
  List.append( i18n("&Full Size Usage") );
  ResizeStyleAction->setItems( List );
  connect( ResizeStyleAction, SIGNAL(activated(int)), this, SLOT(slotSetResizeStyle(int)) );

  ShowOffsetColumnAction = new KToggleAction( i18n("&Line Offset"), Key_F11, this, SLOT(slotToggleOffsetColumn()), AC, "view_lineoffset" );

  // show buffer columns
  ToggleColumnsAction = new KSelectAction( i18n("&Columns"), 0, AC, "togglecolumns" );
  List.clear();
  List.append( i18n("&Values Column") );
  List.append( i18n("&Chars Column") );
  List.append( i18n("&Both Columns") );
  ToggleColumnsAction->setItems( List );
  connect( ToggleColumnsAction, SIGNAL(activated(int)), this, SLOT(slotToggleValueCharColumns(int)) );

  fitActionSettings();

  // set our XML-UI resource file
  setXMLFile( RCFileName );
}


void KHexEditPart::fitActionSettings()
{
  ShowOffsetColumnAction->setChecked( HexEdit->offsetColumnVisible() );
  ShowUnprintableAction->setChecked( HexEdit->showUnprintable() );

  CodingAction->setCurrentItem( (int)HexEdit->coding() );
  EncodingAction->setCurrentItem( KCharCodec::codecNames().findIndex(HexEdit->encodingName()) );

  ResizeStyleAction->setCurrentItem( (int)HexEdit->resizeStyle() );

  ToggleColumnsAction->setCurrentItem( (int)HexEdit->visibleBufferColumns()-1 );
}


bool KHexEditPart::openFile()
{
  Wrapping.open( m_file );
  HexEdit->setDataBuffer( &Wrapping );
  HexEdit->setCursorPosition( 0 );
  HexEdit->selectAll( false );

  return true;
}



void KHexEditPart::slotSelectionChanged()
{
  bool State = HexEdit->hasSelectedData();
  CopyAction->setEnabled( State );
}


void KHexEditPart::slotSelectAll()
{
  HexEdit->selectAll( true );
}


void KHexEditPart::slotUnselect()
{
  HexEdit->selectAll( false );
}


void KHexEditPart::slotSetCoding( int Coding )
{
  HexEdit->setCoding( (KHexEdit::KCoding)Coding );
}

void KHexEditPart::slotSetShowUnprintable()
{
  HexEdit->setShowUnprintable( ShowUnprintableAction->isChecked() );
}

void KHexEditPart::slotToggleOffsetColumn()
{
  HexEdit->toggleOffsetColumn( ShowOffsetColumnAction->isChecked() );
}

void KHexEditPart::slotSetResizeStyle( int ResizeStyle )
{
  HexEdit->setResizeStyle( (KHexEdit::KResizeStyle)ResizeStyle );
}

void KHexEditPart::slotSetEncoding( int Encoding )
{
  HexEdit->setEncoding( KCharCodec::codecNames()[Encoding] );
}

void KHexEditPart::slotToggleValueCharColumns( int VisibleColumns)
{
  HexEdit->showBufferColumns( VisibleColumns+1 );
}

#include "khepart.moc"

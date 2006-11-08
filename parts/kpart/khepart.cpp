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
#include <kselectaction.h>
#include <ktoggleaction.h>
//#include <kglobalsettings.h>
// corelib specific
#include <kcharcodec.h>
// uilib specific
#include <khexedit.h>
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
  setInstance( KHexEditPartFactory::instance() );

  HexEdit = new KHEUI::KHexEdit( &Wrapping, ParentWidget );
  HexEdit->setNoOfBytesPerLine( 16 );
  HexEdit->setBufferSpacing( 3, 4, 10 );
  HexEdit->setShowUnprintable( false );

  // notify the part that this is our internal widget
  setWidget( HexEdit );

  setupActions( BrowserViewWanted );

  connect( HexEdit, SIGNAL(selectionChanged()), SLOT(onSelectionChanged()) );
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
  CopyAction = BrowserViewWanted ? 0 : KStdAction::copy( HexEdit, SLOT(copy()), AC );

  KStdAction::selectAll( this, SLOT(onSelectAll()), AC );
  DeselectAction = KStdAction::deselect( this, SLOT(onUnselect()), AC );

  // value encoding
  CodingAction = new KSelectAction( i18n("&Value Coding"), AC, "view_valuecoding" );
  QStringList List;
  List.append( i18n("&Hexadecimal") );
  List.append( i18n("&Decimal")     );
  List.append( i18n("&Octal")       );
  List.append( i18n("&Binary")      );
  CodingAction->setItems( List );
  connect( CodingAction, SIGNAL(triggered(int)), SLOT(onSetCoding(int)) );

  // document encoding
  EncodingAction = new KSelectAction( i18n("&Char Encoding"), AC, "view_charencoding" );
  EncodingAction->setItems( KHECore::KCharCodec::codecNames() );
  connect( EncodingAction, SIGNAL(triggered(int)), SLOT(onSetEncoding(int)) );

  ShowUnprintableAction = new KToggleAction( i18n("Show &Unprintable Chars (<32)"), AC, "view_showunprintable" );
  connect( ShowUnprintableAction, SIGNAL(activated(int)), SLOT(onSetShowUnprintable()) );

  KStdAction::zoomIn(  HexEdit, SLOT(zoomIn()),  AC );
  KStdAction::zoomOut( HexEdit, SLOT(zoomOut()), AC );

  // resize style
  ResizeStyleAction = new KSelectAction( i18n("&Resize Style"), AC, "resizestyle" );
  List.clear();
  List.append( i18n("&No Resize") );
  List.append( i18n("&Lock Groups") );
  List.append( i18n("&Full Size Usage") );
  ResizeStyleAction->setItems( List );
  connect( ResizeStyleAction, SIGNAL(triggered(int)), SLOT(onSetResizeStyle(int)) );

  ShowOffsetColumnAction = new KToggleAction( i18n("&Line Offset"), AC, "view_lineoffset" );
  ShowOffsetColumnAction->setShortcut( Qt::Key_F11 );
  connect( ShowOffsetColumnAction, SIGNAL(activated(int)), SLOT(onToggleOffsetColumn()) );

  // show buffer columns
  ToggleColumnsAction = new KSelectAction( i18n("&Columns"), AC, "togglecolumns" );
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
  HexEdit->setDataBuffer( &Wrapping );
  HexEdit->setCursorPosition( 0 );
  HexEdit->selectAll( false );

  return true;
}



void KHexEditPart::onSelectionChanged()
{
  const bool HasSelection = HexEdit->hasSelectedData();
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
  HexEdit->setCoding( (KHEUI::KHexEdit::KCoding)Coding );
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
  HexEdit->setResizeStyle( (KHEUI::KHexEdit::KResizeStyle)ResizeStyle );
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

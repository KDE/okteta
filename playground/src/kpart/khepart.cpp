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
#include "khepartfactory.h"
#include "khebrowserextension.h"
#include "khepart.h"

using namespace KHE;

static const char RCFileName[] =    "khexedit2partui.rc";

static const char CodingGroupId[] = "ValueCoding";
static const char ResizeStyleGroupId[] = "ResizeStyle";
static const char EncodingGroupId[] = "CharEncoding";


KHexEditPart::KHexEditPart( QWidget *parentWidget, const char *widgetName,
                            QObject *parent, const char *name )
 : KParts::ReadOnlyPart( parent, name )
{
  setInstance( KHexEditPartFactory::instance() );

  m_HexEdit = new KHexEdit( &m_Wrapping, parentWidget, widgetName );
  m_HexEdit->setNoOfBytesPerLine( 16 );
  m_HexEdit->setBufferSpacing( 3, 4, 10 );
  m_HexEdit->setShowUnprintable( false );

  // notify the part that this is our internal widget
  setWidget( m_HexEdit );

  setupActions();

  connect( m_HexEdit, SIGNAL(copyAvailable(bool)), CopyAction,SLOT(setEnabled(bool)) );
  connect( m_HexEdit, SIGNAL(selectionChanged()),  this,      SLOT(slotSelectionChanged()) );
  connect( m_HexEdit, SIGNAL(bufferChanged()),     this,      SLOT(setModified()) );

  CopyAction->setEnabled( false );

  // plugin to browsers
  new KHexEditBrowserExtension( this, m_HexEdit );
}


KHexEditPart::~KHexEditPart()
{
}


void KHexEditPart::setupActions()
{
  // create our actions
  CopyAction = KStdAction::copy(    m_HexEdit, SLOT(copy()),       actionCollection() );
  KStdAction::selectAll( this, SLOT(slotSelectAll()),     actionCollection() );
  KStdAction::deselect(  this, SLOT(slotUnselect()),      actionCollection() );

  HexCodingAction = new KRadioAction( i18n("&Hexadecimal"), 0, this, SLOT(slotSetCoding()), actionCollection(), "view_hexcoding" );
  DecCodingAction = new KRadioAction( i18n("&Decimal"),     0, this, SLOT(slotSetCoding()), actionCollection(), "view_deccoding" );
  OctCodingAction = new KRadioAction( i18n("&Octal"),       0, this, SLOT(slotSetCoding()), actionCollection(), "view_octcoding" );
  BinCodingAction = new KRadioAction( i18n("&Binary"),      0, this, SLOT(slotSetCoding()), actionCollection(), "view_bincoding" );

  HexCodingAction->setExclusiveGroup( CodingGroupId );
  DecCodingAction->setExclusiveGroup( CodingGroupId );
  OctCodingAction->setExclusiveGroup( CodingGroupId );
  BinCodingAction->setExclusiveGroup( CodingGroupId );

  HexCodingAction->setChecked(true);

  // document encoding
  LocalEncodingAction = new KRadioAction( i18n("&Local"), 0, this, SLOT(slotSetEncoding()), actionCollection(), "view_localencoding" );
  AsciiEncodingAction = new KRadioAction( i18n("&ASCII"), 0, this, SLOT(slotSetEncoding()), actionCollection(), "view_asciiencoding" );

  LocalEncodingAction->setExclusiveGroup( EncodingGroupId );
  AsciiEncodingAction->setExclusiveGroup( EncodingGroupId );

  LocalEncodingAction->setChecked(true);

  ShowUnprintableAction = new KToggleAction( i18n("Show unprintabe chars (<32)"), 0, this, SLOT(slotSetShowUnprintable()), actionCollection(), "view_showunprintable" );
  ShowUnprintableAction->setChecked( false );

  NoResizeAction =      new KRadioAction( i18n("&No Resize"),       0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_noresize" );
  LockGroupsAction =    new KRadioAction( i18n("&Lock groups"),     0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_lockgroups" );
  FullSizeUsageAction = new KRadioAction( i18n("&Full size usage"), 0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_fullsizeusage" );

  NoResizeAction->setExclusiveGroup( ResizeStyleGroupId );
  LockGroupsAction->setExclusiveGroup( ResizeStyleGroupId );
  FullSizeUsageAction->setExclusiveGroup( ResizeStyleGroupId );

  NoResizeAction->setChecked( true );

  KStdAction::zoomIn(  m_HexEdit, SLOT(zoomIn()),   actionCollection() );
  KStdAction::zoomOut( m_HexEdit, SLOT(zoomOut()),  actionCollection() );

  // set our XML-UI resource file
  setXMLFile( RCFileName );
}


bool KHexEditPart::openFile()
{
  m_Wrapping.open( m_file );
  m_HexEdit->setDataBuffer( &m_Wrapping );
  m_HexEdit->moveCursor( KHexEdit::MoveHome );

  return true;
}



void KHexEditPart::slotSelectionChanged()
{
  bool State = m_HexEdit->hasSelectedData();
  CopyAction->setEnabled( State );
}


void KHexEditPart::slotSelectAll()
{
  m_HexEdit->selectAll( true );
}


void KHexEditPart::slotUnselect()
{
  m_HexEdit->selectAll( false );
}


void KHexEditPart::slotSetCoding()
{
  // TODO: find out if there is a way to use the exclusivegroup somehow
  KHexEdit::KCoding Coding;
  if( HexCodingAction->isChecked() )
    Coding = KHexEdit::HexadecimalCoding;
  else if( DecCodingAction->isChecked() )
    Coding = KHexEdit::DecimalCoding;
  else if( OctCodingAction->isChecked() )
    Coding = KHexEdit::OctalCoding;
  else if( BinCodingAction->isChecked() )
    Coding = KHexEdit::BinaryCoding;
  else
    //should not be reached;
    Coding = KHexEdit::HexadecimalCoding;

  m_HexEdit->setCoding( Coding );
}

void KHexEditPart::slotSetShowUnprintable()
{
  m_HexEdit->setShowUnprintable( ShowUnprintableAction->isChecked() );
}

void KHexEditPart::slotSetResizeStyle()
{
  // TODO: find out if there is a way to use the exclusivegroup somehow
  KHexEdit::KResizeStyle ResizeStyle;
  if( NoResizeAction->isChecked() )
    ResizeStyle = KHexEdit::NoResize;
  else if( LockGroupsAction->isChecked() )
    ResizeStyle = KHexEdit::LockGrouping;
  else if( FullSizeUsageAction->isChecked() )
    ResizeStyle = KHexEdit::FullSizeUsage;
  else
    //should not be reached;
    ResizeStyle = KHexEdit::FullSizeUsage;

  m_HexEdit->setResizeStyle( ResizeStyle );
}

void KHexEditPart::slotSetEncoding()
{
  // TODO: find out if there is a way to use the exclusivegroup somehow
  KHexEdit::KEncoding Encoding;
  if( AsciiEncodingAction->isChecked() )
    Encoding = KHexEdit::LocalEncoding;
  else if( LocalEncodingAction->isChecked() )
    Encoding = KHexEdit::ISO8859_1Encoding;
  else
    //should not be reached;
    Encoding = KHexEdit::LocalEncoding;

  m_HexEdit->setEncoding( Encoding );
}


#include "khepart.moc"

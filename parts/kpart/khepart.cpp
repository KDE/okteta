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

static const char RCFileName[] = "khexedit2partui.rc";

static const char CodingGroupId[] =      "ValueCoding";
static const char ResizeStyleGroupId[] = "ResizeStyle";
static const char EncodingGroupId[] =    "CharEncoding";
static const char ShowColumnsGroupId[] = "ShownColumns";


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


void KHexEditPart::setupActions( bool BrowserViewWanted )
{
  // create our actions
  CopyAction = BrowserViewWanted ? 0 : KStdAction::copy( HexEdit, SLOT(copy()), actionCollection() );

  KStdAction::selectAll( this, SLOT(slotSelectAll()),     actionCollection() );
  KStdAction::deselect(  this, SLOT(slotUnselect()),      actionCollection() );

  // value encoding
  HexCodingAction = new KRadioAction( i18n("&Hexadecimal"), 0, this, SLOT(slotSetCoding()), actionCollection(), "view_hexcoding" );
  DecCodingAction = new KRadioAction( i18n("&Decimal"),     0, this, SLOT(slotSetCoding()), actionCollection(), "view_deccoding" );
  OctCodingAction = new KRadioAction( i18n("&Octal"),       0, this, SLOT(slotSetCoding()), actionCollection(), "view_octcoding" );
  BinCodingAction = new KRadioAction( i18n("&Binary"),      0, this, SLOT(slotSetCoding()), actionCollection(), "view_bincoding" );

  HexCodingAction->setExclusiveGroup( CodingGroupId );
  DecCodingAction->setExclusiveGroup( CodingGroupId );
  OctCodingAction->setExclusiveGroup( CodingGroupId );
  BinCodingAction->setExclusiveGroup( CodingGroupId );

  // document encoding
  LocalEncodingAction = new KRadioAction( i18n("&Local"), 0, this, SLOT(slotSetEncoding()), actionCollection(), "view_localencoding" );
  AsciiEncodingAction = new KRadioAction( i18n("&ASCII"), 0, this, SLOT(slotSetEncoding()), actionCollection(), "view_asciiencoding" );

  LocalEncodingAction->setExclusiveGroup( EncodingGroupId );
  AsciiEncodingAction->setExclusiveGroup( EncodingGroupId );

  ShowUnprintableAction = new KToggleAction( i18n("Show &Unprintabe Chars (<32)"), 0, this, SLOT(slotSetShowUnprintable()), actionCollection(), "view_showunprintable" );

  KStdAction::zoomIn(  HexEdit, SLOT(zoomIn()),   actionCollection() );
  KStdAction::zoomOut( HexEdit, SLOT(zoomOut()),  actionCollection() );

  // resize style
  NoResizeAction =      new KRadioAction( i18n("&No Resize"),       0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_noresize" );
  LockGroupsAction =    new KRadioAction( i18n("&Lock Groups"),     0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_lockgroups" );
  FullSizeUsageAction = new KRadioAction( i18n("&Full Size Usage"), 0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_fullsizeusage" );

  NoResizeAction->setExclusiveGroup( ResizeStyleGroupId );
  LockGroupsAction->setExclusiveGroup( ResizeStyleGroupId );
  FullSizeUsageAction->setExclusiveGroup( ResizeStyleGroupId );

  ShowOffsetColumnAction = new KToggleAction( i18n("&Offset Column"), 0, this, SLOT(slotToggleOffsetColumn()), actionCollection(), "settings_showoffsetcolumn" );

  // show buffer columns
  ShowOnlyValueAction =  new KRadioAction( i18n("&Values Column"), 0, this, SLOT(slotToggleValueCharColumns()), actionCollection(), "settings_showonlyvalue" );
  ShowOnlyCharAction =   new KRadioAction( i18n("&Chars Column"),  0, this, SLOT(slotToggleValueCharColumns()), actionCollection(), "settings_showonlychar" );
  ShowValueCharAction = new KRadioAction( i18n("&Both Columns"),   0, this, SLOT(slotToggleValueCharColumns()), actionCollection(), "settings_showvaluechar" );

  ShowOnlyValueAction->setExclusiveGroup( ShowColumnsGroupId );
  ShowOnlyCharAction->setExclusiveGroup( ShowColumnsGroupId );
  ShowValueCharAction->setExclusiveGroup( ShowColumnsGroupId );

  fitActionSettings();

  // set our XML-UI resource file
  setXMLFile( RCFileName );
}


void KHexEditPart::fitActionSettings()
{
  ShowOffsetColumnAction->setChecked( HexEdit->offsetColumnVisible() );
  ShowUnprintableAction->setChecked( HexEdit->showUnprintable() );

  const KHexEdit::KCoding Coding = HexEdit->coding();
  if( Coding == KHexEdit::HexadecimalCoding )
    HexCodingAction->setChecked( true );
  else if( Coding == KHexEdit::DecimalCoding )
    DecCodingAction->setChecked( true );
  else if( Coding == KHexEdit::OctalCoding )
    OctCodingAction->setChecked( true );
  else //if( Coding == KHexEdit::BinaryCoding )
    BinCodingAction->setChecked( true );

  const KHexEdit::KEncoding Encoding = HexEdit->encoding();
  if( Encoding == KHexEdit::LocalEncoding )
    AsciiEncodingAction->setChecked( true );
  else //if( Encoding == KHexEdit::ISO8859_1Encoding )
    LocalEncodingAction->setChecked( true );

  const KHexEdit::KResizeStyle ResizeStyle = HexEdit->resizeStyle();
  if( ResizeStyle == KHexEdit::NoResize )
    NoResizeAction->setChecked( true );
  else if( ResizeStyle == KHexEdit::LockGrouping )
    LockGroupsAction->setChecked( true );
  else //if( ResizeStyle == KHexEdit::FullSizeUsage )
    FullSizeUsageAction->setChecked( true );

  const int Columns = HexEdit->visibleBufferColumns();
  if( Columns == KHexEdit::ValueColumnId )
    ShowOnlyValueAction->setChecked( true );
  else if( Columns == KHexEdit::CharColumnId )
    ShowOnlyCharAction->setChecked( true );
  else //if( Columns == KHexEdit::ValueColumnId | KHexEdit::CharColumnId )
    ShowValueCharAction->setChecked( true );
}


bool KHexEditPart::openFile()
{
  Wrapping.open( m_file );
  HexEdit->setDataBuffer( &Wrapping );
  HexEdit->moveCursor( KHexEdit::MoveHome );

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

  HexEdit->setCoding( Coding );
}

void KHexEditPart::slotSetShowUnprintable()
{
  HexEdit->setShowUnprintable( ShowUnprintableAction->isChecked() );
}

void KHexEditPart::slotToggleOffsetColumn()
{
  HexEdit->toggleOffsetColumn( ShowOffsetColumnAction->isChecked() );
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

  HexEdit->setResizeStyle( ResizeStyle );
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

  HexEdit->setEncoding( Encoding );
}

void KHexEditPart::slotToggleValueCharColumns()
{
  int VisibleColumns;
  if( ShowOnlyValueAction->isChecked() )
    VisibleColumns = KHexEdit::ValueColumnId;
  else if( ShowOnlyCharAction->isChecked() )
    VisibleColumns = KHexEdit::CharColumnId;
  else // if( ShowValueCharAction->isChecked() )
    VisibleColumns = KHexEdit::ValueColumnId | KHexEdit::CharColumnId;

  HexEdit->showBufferColumns( VisibleColumns );
}

#include "khepart.moc"

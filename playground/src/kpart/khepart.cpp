/***************************************************************************
                          khepart.h  -  description
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

// c++ specific
#include <iostream>
// qt specific
#include <qfile.h>
#include <qtextstream.h>
// kde specific
#include <klocale.h>
#include <kinstance.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kfiledialog.h>
#include <kglobalsettings.h>
// app specific
#include "khexedit.h"
#include "khepartfactory.h"
#include "khepart.h"

using namespace KHE;

static const char RCFileName[] =    "khexedit2partui.rc";

static const char CodingGroupId[] = "ColumnCoding";
static const char ResizeStyleGroupId[] = "ResizeStyle";


KHexEditPart::KHexEditPart( QWidget *parentWidget, const char *widgetName,
                            QObject *parent, const char *name )
 : KParts::ReadWritePart( parent, name ),
   m_Buffer( 0L )
{
  setInstance( KHexEditPartFactory::instance() );

  m_HexEdit = new KHexEdit( &m_Wrapping, parentWidget, widgetName );
  m_HexEdit->setNoOfBytesPerLine( 16 );
  m_HexEdit->setBufferSpacing( 3, 4, 10 );
  m_HexEdit->setShowUnprintable( false );

  // notify the part that this is our internal widget
  setWidget( m_HexEdit );

  setupActions();

  // we are read-write by default
  setReadWrite( true );

  // we are not modified since we haven't done anything yet
  setModified( false );
  connect( m_HexEdit, SIGNAL(copyAvailable(bool)), CutAction, SLOT(setEnabled(bool)) );
  connect( m_HexEdit, SIGNAL(copyAvailable(bool)), CopyAction,SLOT(setEnabled(bool)) );
  connect( m_HexEdit, SIGNAL(selectionChanged()),  this,      SLOT(slotSelectionChanged()) );
  connect( m_HexEdit, SIGNAL(bufferChanged()),     this,      SLOT(setModified()) );

  CutAction->setEnabled( false );
  CopyAction->setEnabled( false );
}


KHexEditPart::~KHexEditPart()
{
}


void KHexEditPart::setupActions()
{
  // create our actions
  KStdAction::open(    this, SLOT(fileOpen()),   actionCollection() );
  KStdAction::saveAs(  this, SLOT(fileSaveAs()), actionCollection() );
  SaveAction = KStdAction::save(    this, SLOT(save()),       actionCollection() );

  CutAction =  KStdAction::cut(     m_HexEdit, SLOT(cut()),        actionCollection() );
  CopyAction = KStdAction::copy(    m_HexEdit, SLOT(copy()),       actionCollection() );
  KStdAction::paste(   m_HexEdit, SLOT(paste()),      actionCollection() );
  KStdAction::selectAll( this, SLOT(selectAll()),     actionCollection() );
  KStdAction::deselect(  this, SLOT(unselect()),      actionCollection() );

  HexCodingAction = new KRadioAction( i18n("&Hexadecimal"), 0, this, SLOT(slotSetCoding()), actionCollection(), "view_hexcoding" );
  DecCodingAction = new KRadioAction( i18n("&Decimal"),     0, this, SLOT(slotSetCoding()), actionCollection(), "view_deccoding" );
  OctCodingAction = new KRadioAction( i18n("&Octal"),       0, this, SLOT(slotSetCoding()), actionCollection(), "view_octcoding" );
  BinCodingAction = new KRadioAction( i18n("&Binary"),      0, this, SLOT(slotSetCoding()), actionCollection(), "view_bincoding" );

  HexCodingAction->setExclusiveGroup( CodingGroupId );
  DecCodingAction->setExclusiveGroup( CodingGroupId );
  OctCodingAction->setExclusiveGroup( CodingGroupId );
  BinCodingAction->setExclusiveGroup( CodingGroupId );

  ShowUnprintableAction = new KToggleAction( i18n("Show unprintabe chars(<32)"), 0, this, SLOT(slotSetShowUnprintable()), actionCollection(), "view_showunprintable" );

  NoResizeAction =      new KRadioAction( i18n("&No Resize"),       0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_noresize" );
  LockGroupsAction =    new KRadioAction( i18n("&Lock groups"),     0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_lockgroups" );
  FullSizeUsageAction = new KRadioAction( i18n("&Full size usage"), 0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_fullsizeusage" );

  NoResizeAction->setExclusiveGroup( ResizeStyleGroupId );
  LockGroupsAction->setExclusiveGroup( ResizeStyleGroupId );
  FullSizeUsageAction->setExclusiveGroup( ResizeStyleGroupId );

  KStdAction::zoomIn(  m_HexEdit, SLOT(zoomIn()),   actionCollection() );
  KStdAction::zoomOut( m_HexEdit, SLOT(zoomOut()),  actionCollection() );

  // set our XML-UI resource file
  setXMLFile( RCFileName );
}


void KHexEditPart::setReadWrite( bool rw )
{
  // notify your internal widget of the read-write state
//   m_HexEdit->setReadOnly( !rw );
//
//   if( m_HexEdit->isReadOnly() )
//     disconnect( m_HexEdit, SIGNAL(bufferChanged()), this,     SLOT(setModified()) );
//   else
//     connect( m_HexEdit, SIGNAL(bufferChanged()), this,     SLOT(setModified()) );

  ReadWritePart::setReadWrite( rw );
}


void KHexEditPart::setModified( bool Modified )
{
  // if so, we either enable or disable it based on the current state
  SaveAction->setEnabled( Modified );

  // in any event, we want our parent to do it's thing
  ReadWritePart::setModified( Modified );
}


bool KHexEditPart::openFile()
{
#if 0
  // m_file is always local so we can use QFile on it
  QFile File( m_file );
  if( !File.open(IO_ReadOnly|IO_Raw) )
    return false;

  delete [] m_Buffer;
  int S = File.size() + 1;
  m_Buffer = new char[S];

  if( File.size() > 0 )
  {
    uint offset = 0;
    uint remaining = File.size();
    while( remaining > 0 )
    {
      uint blockSize = remaining > 100000 ? 100000 : remaining;
      File.readBlock( &m_Buffer[offset], blockSize );

      offset    += blockSize;
      remaining -= blockSize;
   }
  }
  m_Wrapping.set( m_Buffer, File.size() );

#endif
  m_Wrapping.open( m_file );
  m_HexEdit->setDataBuffer( &m_Wrapping );
  m_HexEdit->moveCursor( KHexEdit::MoveHome );

  // just for fun, set the status bar
  emit setStatusBarText( m_url.prettyURL() );

  return true;
}


bool KHexEditPart::saveFile()
{
  // if we aren't read-write, return immediately
  if( !isReadWrite() )
    return false;

//   // m_file is always local, so we use QFile
//   QFile file( m_file );
//   if( !file.open(IO_WriteOnly) )
//     return false;
//
//   // use QTextStream to dump the text to the file
//   QTextStream stream( &file );
//   stream << m_widget->text();
//
//   file.close();

  return true;
}


void KHexEditPart::fileOpen()
{
  // this slot is called whenever the m_file->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
  // button is clicked
  QString file_name = KFileDialog::getOpenFileName();

  if( !file_name.isEmpty() )
    openURL(file_name);
}


void KHexEditPart::fileSaveAs()
{
  // this slot is called whenever the m_file->Save As menu is selected,
  QString file_name = KFileDialog::getSaveFileName();
  if( !file_name.isEmpty() )
    saveAs( file_name );
}


void KHexEditPart::selectAll()
{
  m_HexEdit->selectAll( true );
}


void KHexEditPart::unselect()
{
  m_HexEdit->selectAll( false );
}


void KHexEditPart::slotSelectionChanged()
{
  bool State = m_HexEdit->hasSelectedData();
  CutAction->setEnabled( State );
  CopyAction->setEnabled( State );
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
#include "khepart.moc"

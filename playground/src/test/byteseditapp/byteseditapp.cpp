/***************************************************************************
                          byteseditapp.cpp  -  description
                             -------------------
    begin                : Tue Jul 29 2003
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

// c specific
#include <string.h>
// c++ specific
#include <iostream>
// qt specific
#include <qpainter.h>
#include <qpaintdevicemetrics.h>
#include <qlayout.h>
// kde specific
#include <kurl.h>
#include <ktrader.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <krun.h>
#include <klocale.h>
#include <kglobalsettings.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kconfig.h>
#include <kurl.h>
#include <kstandarddirs.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
// new kde specific
#include <khexedit/byteseditinterface.h>
#include <khexedit/valuecolumninterface.h>
#include <khexedit/charcolumninterface.h>
#include <khexedit/clipboardinterface.h>
#include <khexedit/zoominterface.h>
// app specific
#include "byteseditapp.h"


static const char PathToIcon[] = "khe/pics/";
static const char *CodingGroupId = "ColumnCoding";
static const char *ResizeStyleGroupId = "ResizeStyle";
static const char *RCFileName = "byteseditappui.rc";

BytesEditApp::BytesEditApp()
    : KMainWindow( 0, "BytesEditApp" )
{
#if 1
  static const int BufferSize = 893;
  Buffer = new char[BufferSize];
  memset( Buffer, '#', BufferSize );
  memcpy( &Buffer[10], "This is a test text.", 20 );
  memcpy( &Buffer[873], "This is a test text.", 20 );
#else
  static const int BufferSize = 0;
  Buffer = new char[BufferSize];
#endif

  BytesEditWidget = KHE::createBytesEditWidget( this, "BytesEditWidget" );
  // was kdeutils installed, so the widget could be found?
  if( BytesEditWidget )
  {
    // fetch the editor interface
//     KBytesEditInterface *BytesEdit = static_cast<KBytesEditInterface *>( BytesEditWidget->qt_cast( "KBytesEditInterface" ) );
    BytesEdit = KHE::bytesEditInterface( BytesEditWidget );
    Q_ASSERT( BytesEdit ); // This should not fail!

    // now use the editor.
//    BytesEdit->setData( Buffer, BufferSize, -1, false );
//    BytesEdit->setMaxDataSize( BufferSize );
    BytesEdit->setMaxDataSize( 25 );
    BytesEdit->setReadOnly( false );
    BytesEdit->setAutoDelete( true );
#if 0
    KHE::ValueColumnInterface *ValueColumn = hexColumnInterface( BytesEditWidget );
    if( ValueColumn )
    {
      Layout->setResizeStyle( KBytesEditInterface::LockGrouping );
//     BytesEdit->setNoOfBytesPerLine( 16 );
//     BytesEdit->setResizeStyle( KHE::KBytesEdit::LockGrouping );
    }
#endif
    KHE::ValueColumnInterface *ValueColumn = KHE::valueColumnInterface( BytesEditWidget );
    if( ValueColumn )
    {
      ValueColumn->setResizeStyle( KHE::ValueColumnInterface::LockGrouping );
      ValueColumn->setCoding( KHE::ValueColumnInterface::DecimalCoding );
//      ValueColumn->setCoding( ValueColumnInterface::BinaryCoding );
      ValueColumn->setByteSpacingWidth( 2 );
      ValueColumn->setNoOfGroupedBytes( 4 );
      ValueColumn->setGroupSpacingWidth( 12 );
    }

    KHE::CharColumnInterface *CharColumn = KHE::charColumnInterface( BytesEditWidget );
    if( CharColumn )
    {
      CharColumn->setShowUnprintable( false );
      CharColumn->setSubstituteChar( '*' );
      CharColumn->setEncoding( KHE::CharColumnInterface::ISO8859_1Encoding/*/LocalEncoding*/ );
    }
    Clipboard = KHE::clipboardInterface( BytesEditWidget );
    Zoom = KHE::zoomInterface( BytesEditWidget );
  }
  else {
    // Don't offer the editor widget.
  }
  // tell the KMainWindow that this is indeed the main widget
  setCentralWidget( BytesEditWidget );

  // then, setup our actions
  setupActions();

  // and a status bar
  statusBar()->show();

  // apply the saved mainwindow settings, if any, and ask the mainwindow
  // to automatically save settings if changed: window size, toolbar
  // position, icon size, etc.
  setAutoSaveSettings();
}

BytesEditApp::~BytesEditApp()
{
}


void BytesEditApp::setupActions()
{
  ReadOnlyAction =
    new KToggleAction( i18n("ReadOnly"), "edit", 0,
                       this, SLOT(setReadOnly()), actionCollection(), "file_setreadonly" );
//   KStdAction::print(this, SLOT(filePrint()), actionCollection());
  KStdAction::quit(    kapp, SLOT(quit()),       actionCollection());

  if( Clipboard )
  {
    CutAction =  KStdAction::cut(  BytesEditWidget, SLOT(cut()),  actionCollection() );
    CopyAction = KStdAction::copy( BytesEditWidget, SLOT(copy()), actionCollection() );
    KStdAction::paste( BytesEditWidget, SLOT(paste()), actionCollection() );
  }
  else
  {
    CutAction =  0;
    CopyAction = 0;
  }
//   KStdAction::selectAll( this, SLOT(selectAll()),     actionCollection() );
//   KStdAction::deselect(  this, SLOT(unselect()),      actionCollection() );

  HexCodingAction = new KRadioAction( i18n("&Hexadecimal"), 0, this, SLOT(slotSetCoding()), actionCollection(), "view_hexcoding" );
  DecCodingAction = new KRadioAction( i18n("&Decimal"),     0, this, SLOT(slotSetCoding()), actionCollection(), "view_deccoding" );
  OctCodingAction = new KRadioAction( i18n("&Octal"),       0, this, SLOT(slotSetCoding()), actionCollection(), "view_octcoding" );
  BinCodingAction = new KRadioAction( i18n("&Binary"),      0, this, SLOT(slotSetCoding()), actionCollection(), "view_bincoding" );

  HexCodingAction->setExclusiveGroup( CodingGroupId );
  DecCodingAction->setExclusiveGroup( CodingGroupId );
  OctCodingAction->setExclusiveGroup( CodingGroupId );
  BinCodingAction->setExclusiveGroup( CodingGroupId );

  ShowUnprintableAction = new KToggleAction( i18n("Show unprintabe chars(<32)"), 0, this, SLOT(slotSetShowUnprintable()), actionCollection(), "view_showunprintable" );

  if( Zoom )
  {
    KStdAction::zoomIn(  BytesEditWidget, SLOT(zoomIn()),   actionCollection() );
    KStdAction::zoomOut( BytesEditWidget, SLOT(zoomOut()),  actionCollection() );
  }

//   connect( m_view, SIGNAL(cutAvailable(bool)),  CutAction, SLOT(setEnabled(bool)) );
//   connect( m_view, SIGNAL(copyAvailable(bool)), CopyAction,SLOT(setEnabled(bool)) );
//   connect( m_view, SIGNAL(selectionChanged()),  this,      SLOT(slotSelectionChanged()));

  KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
  KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());

  NoResizeAction =      new KRadioAction( i18n("&No Resize"),       0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_noresize" );
  LockGroupsAction =    new KRadioAction( i18n("&Lock groups"),     0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_lockgroups" );
  FullSizeUsageAction = new KRadioAction( i18n("&Full size usage"), 0, this, SLOT(slotSetResizeStyle()), actionCollection(), "settings_fullsizeusage" );

  NoResizeAction->setExclusiveGroup( ResizeStyleGroupId );
  LockGroupsAction->setExclusiveGroup( ResizeStyleGroupId );
  FullSizeUsageAction->setExclusiveGroup( ResizeStyleGroupId );

  // set default values
  CutAction->setEnabled( false );
  CopyAction->setEnabled( false );
  ReadOnlyAction->setEnabled( true );

  createGUI( RCFileName );
}

void BytesEditApp::saveProperties(KConfig */*config*/)
{
    // the 'config' object points to the session managed
    // config file.  anything you write here will be available
    // later when this app is restored

//     if (!m_view->currentURL().isNull())
//         config->writeEntry("lastURL", m_view->currentURL());
}

void BytesEditApp::readProperties(KConfig */*config*/)
{
    // the 'config' object points to the session managed
    // config file.  this function is automatically called whenever
    // the app is being restored.  read in here whatever you wrote
    // in 'saveProperties'

//     QString url = config->readEntry("lastURL");

//     if (!url.isNull())
//         m_view->openURL(KURL(url));
}


void BytesEditApp::setReadOnly()
{
  bool ReadOnly = ReadOnlyAction->isChecked();
  BytesEdit->setReadOnly( ReadOnly );
//   ReadOnlyAction->setIconSet( MainBarIcon(ReadOnly?"lock":"edit") );
}



void BytesEditApp::optionsConfigureKeys()
{
    KKeyDialog::configure( actionCollection() );
}

void BytesEditApp::optionsConfigureToolbars()
{
    // use the standard toolbar editor
    saveMainWindowSettings(KGlobal::config(), autoSaveGroup());
}

void BytesEditApp::newToolbarConfig()
{
    // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
    // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
    createGUI( RCFileName );

    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
}


void BytesEditApp::changeStatusbar(const QString& text)
{
    // display the text on the statusbar
    statusBar()->message(text);
}

void BytesEditApp::changeCaption(const QString& text)
{
    // display the text on the caption
    setCaption(text);
}

void BytesEditApp::slotSetCoding()
{
  // TODO: find out if there is a way to use the exclusivegroup somehow
  KHE::ValueColumnInterface::KCoding Coding;
  if( HexCodingAction->isChecked() )
    Coding = KHE::ValueColumnInterface::HexadecimalCoding;
  else if( DecCodingAction->isChecked() )
    Coding = KHE::ValueColumnInterface::DecimalCoding;
  else if( OctCodingAction->isChecked() )
    Coding = KHE::ValueColumnInterface::OctalCoding;
  else if( BinCodingAction->isChecked() )
    Coding = KHE::ValueColumnInterface::BinaryCoding;
  else
    //should not be reached;
    Coding = KHE::ValueColumnInterface::HexadecimalCoding;

  KHE::valueColumnInterface(BytesEditWidget)->setCoding( Coding );
}

void BytesEditApp::slotSetShowUnprintable()
{
  KHE::charColumnInterface(BytesEditWidget)->setShowUnprintable( ShowUnprintableAction->isChecked() );
}

void BytesEditApp::slotSetResizeStyle()
{
  // TODO: find out if there is a way to use the exclusivegroup somehow
  KHE::ValueColumnInterface::KResizeStyle ResizeStyle;
  if( NoResizeAction->isChecked() )
    ResizeStyle = KHE::ValueColumnInterface::NoResize;
  else if( LockGroupsAction->isChecked() )
    ResizeStyle = KHE::ValueColumnInterface::LockGrouping;
  else if( FullSizeUsageAction->isChecked() )
    ResizeStyle = KHE::ValueColumnInterface::FullSizeUsage;
  else
    //should not be reached;
    ResizeStyle = KHE::ValueColumnInterface::FullSizeUsage;

  KHE::valueColumnInterface(BytesEditWidget)->setResizeStyle( ResizeStyle );
}

#include "byteseditapp.moc"

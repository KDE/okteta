/***************************************************************************
                          khexedit2.cpp  -  description
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


// qt specific
#include <qdragobject.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>

// kde specific
// #include <kprinter.h>
#include <klibloader.h>
#include <kmessagebox.h>
#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurlrequesterdlg.h>
#include <kedittoolbar.h>
#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kapplication.h>
// app specific
#include "pref.h"
#include "khexedit2.h"


static const char UIRCFileName[] =   "khexedit2ui.rc";
static const char LibFileName[] =    "libkhexedit2part";
// static const char ConfigFileName[] = "khexedit2.rc";
static const char PartName[] =       "khexedit2part";

// static const char IdLastUrl[] = "lastURL";


KHexEdit2::KHexEdit2()
 : KParts::DockMainWindow( 0, "KHexEdit2App" ),
   m_part( 0L )
//    m_printer( 0 )
{
  // accept dnd
  setAcceptDrops( true );

  KDockWidget* mainDock = createDockWidget( "MainDockWidget", QPixmap(), 0L, "main_dock_widget" );

//   m_view = new KHexEdit2View( mainDock );
//   mainDock->setWidget( m_view );

  // allow others to dock to the 4 sides
  mainDock->setDockSite( KDockWidget::DockCorner );
  // forbit docking abilities of mainDock itself
  mainDock->setEnableDocking( KDockWidget::DockNone );

  setView( mainDock ); // central widget in a KDE mainwindow
  setMainDockWidget( mainDock ); // master dockwidget

//   was: // tell the KMainWindow that this is indeed the main widget
//   setCentralWidget( m_view );

  // this routine will find and load our Part.  it finds the Part by
  // name which is a bad idea usually.. but it's alright in this
  // case since our Part is made for this Shell
  KLibFactory *factory = KLibLoader::self()->factory( LibFileName );
  if( factory )
  {
    // now that the Part is loaded, we cast it to a Part to get
    // our hands on it
    m_part = static_cast<KParts::ReadWritePart *>
             ( factory->create(this,PartName,"KParts::ReadWritePart") );

    if( m_part )
    {
      // tell the KParts::MainWindow that this is indeed the main widget
      setCentralWidget( m_part->widget() );
    }
  }
  else
  {
    // if we couldn't find our Part, we exit since the Shell by
    // itself can't do anything useful
    KMessageBox::error( this, i18n("Could not find the KHexEdit2 part.") );
    kapp->quit();
    // we return here, cause kapp->quit() only means "exit the
    // next time we enter the event loop...
    return;
  }

  // set the shell's ui resource file
  setXMLFile( UIRCFileName );

  // then, setup our actions
  setupActions();

  // and a status bar
  statusBar()->show();

  // apply the saved mainwindow settings, if any, and ask the mainwindow
  // to automatically save settings if changed: window size, toolbar
  // position, icon size, etc.
  setAutoSaveSettings();

  // allow the view to change the statusbar and caption
//   connect( m_part, SIGNAL(signalChangeStatusbar(const QString&)),
//            this,   SLOT(changeStatusbar(const QString&)));
//   connect( m_part, SIGNAL(signalChangeCaption(const QString&)),
//            this,   SLOT(changeCaption(const QString&)) );
}


KHexEdit2::~KHexEdit2()
{
}



void KHexEdit2::load(const KURL& url)
{
  m_part->openURL( url );
}
// void KHexEdit2::load( const KURL& url )
// {
//   QString target;
  // the below code is what you should normally do.  in this
  // example case, we want the url to our own.  you probably
  // want to use this code instead for your app

//   #if 0
  // download the contents
//   if (KIO::NetAccess::download(url, target))
//   {
      // set our caption
//       setCaption(url);

      // load in the file (target is always local)
//       loadFile(target);

      // and remove the temp file
//       KIO::NetAccess::removeTempFile(target);
//   }
//   #endif

//   setCaption( url.url() );
//   m_view->openURL( url );
// }


void KHexEdit2::setupActions()
{
  KStdAction::openNew( this, SLOT(fileNew()),   actionCollection() );
  KStdAction::open(    this, SLOT(fileOpen()),  actionCollection() );
//   KStdAction::save(    this, SLOT(fileSave()),  actionCollection() );
//   KStdAction::saveAs(  this, SLOT(fileSaveAs()),actionCollection() );
//   KStdAction::print(   this, SLOT(filePrint()), actionCollection() );
  KStdAction::quit(    kapp, SLOT(quit()),      actionCollection() );

  m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
  m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

  KStdAction::keyBindings(       this, SLOT(optionsConfigureKeys()),     actionCollection() );
  KStdAction::configureToolbars( this, SLOT(optionsConfigureToolbars()), actionCollection() );
  KStdAction::preferences(       this, SLOT(optionsPreferences()),       actionCollection() );

  // this doesn't do anything useful.  it's just here to illustrate
  // how to insert a custom menu and menu item
//   KAction *custom = new KAction( i18n("Cus&tom Menuitem"), 0,
//                                  this, SLOT(optionsPreferences()),
//                                  actionCollection(), "custom_action" );

  // and integrate the part's GUI with the shell's
  createGUI( m_part );
}


void KHexEdit2::saveProperties( KConfig *Config )
{
  // the 'config' object points to the session managed
  // config file.  anything you write here will be available
  // later when this app is restored

//   if( !m_view->currentURL().isNull() )
//     Config->writeEntry( IdLastUrl, m_view->currentURL() );

  writeDockConfig( Config );
}


void KHexEdit2::readProperties( KConfig *Config )
{
  // the 'config' object points to the session managed
  // config file.  this function is automatically called whenever
  // the app is being restored.  read in here whatever you wrote
  // in 'saveProperties'

//   QString url = Config->readEntry( IdLastUrl );

//   if( !url.isNull() )
//     m_view->openURL( KURL(url) );

  readDockConfig( Config );
}


// void KHexEdit2::dragEnterEvent( QDragEnterEvent *event )
// {
//   // accept uri drops only
//   event->accept( QUriDrag::canDecode(event) );
// }


// void KHexEdit2::dropEvent( QDropEvent *event )
// {
//   // this is a very simplistic implementation of a drop event.  we
//   // will only accept a dropped URL.  the Qt dnd code can do *much*
//   // much more, so please read the docs there
//   QStrList uri;
//
//   // see if we can decode a URI.. if not, just ignore it
//   if( QUriDrag::decode(event,uri) )
//   {
//     // okay, we have a URI.. process it
//     QString url, target;
//     url = uri.first();
//
//     // load in the file
//     load(KURL(url));
//   }
// }


void KHexEdit2::fileNew()
{
  // this slot is called whenever the File->New menu is selected,
  // the New shortcut is pressed (usually CTRL+N) or the New toolbar
  // button is clicked

  // About this function, the style guide (
  // http://developer.kde.org/documentation/standards/kde/style/basics/index.html )
  // says that it should open a new window if the document is _not_
  // in its initial state.  This is what we do here..
  if ( ! m_part->url().isEmpty() || m_part->isModified() )
  {
    (new KHexEdit2)->show();
  };
}


void KHexEdit2::fileOpen()
{
  // this slot is called whenever the File->Open menu is selected,
  // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
  // button is clicked
  KURL url = KFileDialog::getOpenURL( QString::null, QString::null, this );

  if( !url.isEmpty() )
  {
    // About this function, the style guide (
    // http://developer.kde.org/documentation/standards/kde/style/basics/index.html )
    // says that it should open a new window if the document is _not_
    // in its initial state.  This is what we do here..
    if ( m_part->url().isEmpty() && ! m_part->isModified() )
    {
      // we open the file in this window...
      load( url );
    }
    else
    {
      // we open the file in a new window...
      KHexEdit2* newWin = new KHexEdit2;
      newWin->load( url );
      newWin->show();
    }
  }
}

// void KHexEdit2::fileSave()
// {
  // this slot is called whenever the File->Save menu is selected,
  // the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
  // button is clicked

  // save the current file
// }


// void KHexEdit2::fileSaveAs()
// {
  // this slot is called whenever the File->Save As menu is selected,
//   KURL file_url = KFileDialog::getSaveURL();
//   if (!file_url.isEmpty() && !file_url.isMalformed())
//   {
      // save your info, here
//   }
// }

#if 0
void KHexEdit2::filePrint()
{
  // this slot is called whenever the File->Print menu is selected,
  // the Print shortcut is pressed (usually CTRL+P) or the Print toolbar
  // button is clicked
  if( !m_printer ) m_printer = new KPrinter;
  if( m_printer->setup(this) )
  {
    // setup the printer.  with Qt, you always "print" to a
    // QPainter.. whether the output medium is a pixmap, a screen,
    // or paper
    QPainter p;
    p.begin(m_printer);

    // we let our view do the actual printing
    QPaintDeviceMetrics metrics(m_printer);
    m_view->print( &p, metrics.height(), metrics.width() );

    // and send the result to the printer
    p.end();
  }
}
#endif

void KHexEdit2::optionsShowToolbar()
{
  // this is all very cut and paste code for showing/hiding the
  // toolbar
  if( m_toolbarAction->isChecked() )
    toolBar()->show();
  else
    toolBar()->hide();
}


void KHexEdit2::optionsShowStatusbar()
{
// this is all very cut and paste code for showing/hiding the
// statusbar
  if( m_statusbarAction->isChecked() )
    statusBar()->show();
  else
    statusBar()->hide();
}


void KHexEdit2::optionsConfigureKeys()
{
  KKeyDialog::configureKeys( actionCollection(), UIRCFileName );
}


void KHexEdit2::optionsConfigureToolbars()
{
  saveMainWindowSettings( KGlobal::config(), autoSaveGroup() );

  // use the standard toolbar editor
  KEditToolbar Dialog( factory() );
  connect( &Dialog, SIGNAL(newToolbarConfig()), this, SLOT(applyNewToolbarConfig()) );
  Dialog.exec();
}


void KHexEdit2::applyNewToolbarConfig()
{
  // this slot is called when user clicks "Ok" or "Apply" in the toolbar editor.
  // recreate our GUI, and re-apply the settings (e.g. "text under icons", etc.)
//   createGUI();

  applyMainWindowSettings( KGlobal::config(), autoSaveGroup() );
}


void KHexEdit2::optionsPreferences()
{
  // popup some sort of preference dialog, here
  KHexEdit2Preferences dlg;
  if( dlg.exec() )
  {
      // redo your settings
  }
}


void KHexEdit2::changeStatusbar( const QString& text )
{
  // display the text on the statusbar
  statusBar()->message( text );
}

void KHexEdit2::changeCaption( const QString& text )
{
  // display the text on the caption
  setCaption( text );
}


// void KHexEdit2::()
// {
//   m_SearchWidget = createDockWidget( "Intially left one", anyOtherPixmap, 0L, i18n("The left dockwidget"));
//   AnotherWidget* aw = new AnotherWidget( dockLeft );
//   dockLeft->setWidget( aw );
//   dockLeft->manualDock( getMainDockWidget(), KDockWidget::DockLeft, 20 );
// }

#include "khexedit2.moc"

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

#include "byteseditapp.h"
#include "pref.h"

#include <qdragobject.h>
#include <kprinter.h>
#include <qpainter.h>
#include <qpaintdevicemetrics.h>

#include <kglobal.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kkeydialog.h>
#include <kaccel.h>
#include <kio/netaccess.h>
#include <kfiledialog.h>
#include <kconfig.h>
#include <kurl.h>
#include <kurlrequesterdlg.h>
#include <kstandarddirs.h>

#include <kedittoolbar.h>

#include <kstdaccel.h>
#include <kaction.h>
#include <kstdaction.h>


static const char PathToIcon[] = "khe/pics/";


BytesEditApp::BytesEditApp()
    : KMainWindow( 0, "BytesEditApp" ),
      m_view(new BytesEditAppView(this))
{
    // accept dnd
    setAcceptDrops(true);

    // tell the KMainWindow that this is indeed the main widget
    setCentralWidget(m_view);

    // then, setup our actions
    setupActions();

    // and a status bar
    statusBar()->show();

    // apply the saved mainwindow settings, if any, and ask the mainwindow
    // to automatically save settings if changed: window size, toolbar
    // position, icon size, etc.
    setAutoSaveSettings();

    // allow the view to change the statusbar and caption
    connect(m_view, SIGNAL(signalChangeStatusbar(const QString&)),
            this,   SLOT(changeStatusbar(const QString&)));
    connect(m_view, SIGNAL(signalChangeCaption(const QString&)),
            this,   SLOT(changeCaption(const QString&)));

}

BytesEditApp::~BytesEditApp()
{
}

void BytesEditApp::load(const KURL& url)
{
    QString target;
    // the below code is what you should normally do.  in this
    // example case, we want the url to our own.  you probably
    // want to use this code instead for your app

    #if 0
    // download the contents
    if (KIO::NetAccess::download(url, target))
    {
        // set our caption
        setCaption(url);

        // load in the file (target is always local)
        loadFile(target);

        // and remove the temp file
        KIO::NetAccess::removeTempFile(target);
    }
    #endif

    setCaption(url.url());
//     m_view->openURL(url);
}

void BytesEditApp::setupActions()
{
  KStdAction::openNew( this, SLOT(fileNew()),    actionCollection() );
  KStdAction::open(    this, SLOT(fileOpen()),   actionCollection() );
  KStdAction::save(    this, SLOT(fileSave()),   actionCollection() );
  KStdAction::saveAs(  this, SLOT(fileSaveAs()), actionCollection() );
  ReadOnlyAction =
    new KToggleAction( i18n("ReadOnly"), "edit", 0,
                       this, SLOT(fileSetReadOnly()), actionCollection(), "file_setreadonly" );
//   KStdAction::print(this, SLOT(filePrint()), actionCollection());
  KStdAction::quit(    kapp, SLOT(quit()),       actionCollection());

  CutAction =  KStdAction::cut(     m_view, SLOT(cut()),        actionCollection() );
  CopyAction = KStdAction::copy(    m_view, SLOT(copy()),       actionCollection() );
  KStdAction::paste(   m_view, SLOT(paste()),    actionCollection() );

  KStdAction::zoomIn(  m_view, SLOT(zoomIn()),   actionCollection() );
  KStdAction::zoomOut( m_view, SLOT(zoomOut()),  actionCollection() );

  connect( m_view, SIGNAL(cutAvailable(bool)),  CutAction, SLOT(setEnabled(bool)) );
  connect( m_view, SIGNAL(copyAvailable(bool)), CopyAction,SLOT(setEnabled(bool)) );
//   connect( m_view, SIGNAL(selectionChanged()),  this,      SLOT(slotSelectionChanged()));

  m_toolbarAction = KStdAction::showToolbar(this, SLOT(optionsShowToolbar()), actionCollection());
  m_statusbarAction = KStdAction::showStatusbar(this, SLOT(optionsShowStatusbar()), actionCollection());

  KStdAction::keyBindings(this, SLOT(optionsConfigureKeys()), actionCollection());
  KStdAction::configureToolbars(this, SLOT(optionsConfigureToolbars()), actionCollection());
  KStdAction::preferences(this, SLOT(optionsPreferences()), actionCollection());

  CutAction->setEnabled( false );
  CopyAction->setEnabled( false );
  ReadOnlyAction->setEnabled( true );

  createGUI();
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

void BytesEditApp::dragEnterEvent(QDragEnterEvent *event)
{
    // accept uri drops only
    event->accept(QUriDrag::canDecode(event));
}

void BytesEditApp::dropEvent(QDropEvent *event)
{
    // this is a very simplistic implementation of a drop event.  we
    // will only accept a dropped URL.  the Qt dnd code can do *much*
    // much more, so please read the docs there
    QStrList uri;

    // see if we can decode a URI.. if not, just ignore it
    if (QUriDrag::decode(event, uri))
    {
        // okay, we have a URI.. process it
        QString url, target;
        url = uri.first();

        // load in the file
        load(KURL(url));
    }
}

void BytesEditApp::fileNew()
{
    // this slot is called whenever the File->New menu is selected,
    // the New shortcut is pressed (usually CTRL+N) or the New toolbar
    // button is clicked

    // create a new window
    (new BytesEditApp)->show();
}

void BytesEditApp::fileOpen()
{
    // this slot is called whenever the File->Open menu is selected,
    // the Open shortcut is pressed (usually CTRL+O) or the Open toolbar
    // button is clicked
/*
    // this brings up the generic open dialog
    KURL url = KURLRequesterDlg::getURL(QString::null, this, i18n("Open Location") );
*/
    // standard filedialog
//     KURL url = KFileDialog::getOpenURL(QString::null, QString::null, this, i18n("Open Location"));
//     if (!url.isEmpty())
//         m_view->openURL(url);
}

void BytesEditApp::fileSave()
{
    // this slot is called whenever the File->Save menu is selected,
    // the Save shortcut is pressed (usually CTRL+S) or the Save toolbar
    // button is clicked

    // save the current file
}

void BytesEditApp::fileSaveAs()
{
    // this slot is called whenever the File->Save As menu is selected,
    KURL file_url = KFileDialog::getSaveURL();
    if (!file_url.isEmpty() && !file_url.isMalformed())
    {
        // save your info, here
    }
}

void BytesEditApp::fileSetReadOnly()
{
  bool ReadOnly = ReadOnlyAction->isChecked();
  m_view->setReadOnly( ReadOnly );
//   ReadOnlyAction->setIconSet( MainBarIcon(ReadOnly?"lock":"edit") );
}


void BytesEditApp::optionsShowToolbar()
{
    // this is all very cut and paste code for showing/hiding the
    // toolbar
    if (m_toolbarAction->isChecked())
        toolBar()->show();
    else
        toolBar()->hide();
}

void BytesEditApp::optionsShowStatusbar()
{
    // this is all very cut and paste code for showing/hiding the
    // statusbar
    if (m_statusbarAction->isChecked())
        statusBar()->show();
    else
        statusBar()->hide();
}

void BytesEditApp::optionsConfigureKeys()
{
    KKeyDialog::configureKeys(actionCollection(), "byteseditappui.rc");
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
    createGUI();

    applyMainWindowSettings(KGlobal::config(), autoSaveGroup());
}

void BytesEditApp::optionsPreferences()
{
    // popup some sort of preference dialog, here
    BytesEditAppPreferences dlg;
    if (dlg.exec())
    {
        // redo your settings
    }
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

/***************************************************************************
                          khexedit2.h  -  description
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


#ifndef KHEXEDIT2_H_
#define KHEXEDIT2_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// kde specific
#include <kparts/dockmainwindow.h>

//#include "khexedit2view.h"

// forward declarations
// class KPrinter;
class KToggleAction;
class KURL;

/**
 * This is the application "Shell".  It has a menubar, toolbar, and
 * statusbar but relies on the "Part" to do all the real work.
 *
 * @short Application Shell
 * @author Friedrich W. H.  Kossebau <Friedrich.W.H@Kossebau.de>
 * @version 0.1.0
 */
class KHexEdit2 : public KParts::DockMainWindow
{
    Q_OBJECT

  public:
    /**
     * Default Constructor
     */
    KHexEdit2();

    /**
     * Default Destructor
     */
    virtual ~KHexEdit2();

    /**
     * Use this method to load whatever file/URL you have
     */
    void load( const KURL& url );


  protected:
    /**
     * Overridden virtuals for Qt drag 'n drop (XDND)
     */
//    virtual void dragEnterEvent( QDragEnterEvent *event );
//    virtual void dropEvent( QDropEvent *event ); 


  protected:
    /**
     * This method is called when it is time for the app to save its
     * properties for session management purposes.
     */
    void saveProperties( KConfig * );

    /**
     * This method is called when this app is restored.  The KConfig
     * object points to the session management config file that was saved
     * with @ref saveProperties
     */
    void readProperties( KConfig * );


  private slots:
    void fileNew();
    void fileOpen();
//    void fileSave();
//    void fileSaveAs();
//    void filePrint();
    void optionsShowToolbar();
    void optionsShowStatusbar();
    void optionsConfigureKeys();
    void optionsConfigureToolbars();
    void optionsPreferences();

    void applyNewToolbarConfig();

    void changeStatusbar(const QString& text);
    void changeCaption(const QString& text);

  private:
    void setupAccel();
    void setupActions();

  private:
    KParts::ReadWritePart *m_part;
//     KHexEdit2View *m_view;
//     KDockWidget* m_SearchWidget;

    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif

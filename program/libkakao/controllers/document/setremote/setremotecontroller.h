/***************************************************************************
                          setremotecontroller.h  -  description
                             -------------------
    begin                : Sat Nov 17 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef SETREMOTECONTROLLER_H
#define SETREMOTECONTROLLER_H


// lib
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KAction;
class KXmlGuiWindow;
class KAbstractDocument;
class KDocumentLoaderManager;

class SetRemoteController : public KViewController
{
  Q_OBJECT

  public:
    SetRemoteController( KDocumentLoaderManager *loaderManager, KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void saveAs();

  protected:
    KXmlGuiWindow *mMainWindow;
    KAction *mSaveAsAction;

    KDocumentLoaderManager *mLoaderManager;
    KAbstractDocument *mDocument;
};

#endif

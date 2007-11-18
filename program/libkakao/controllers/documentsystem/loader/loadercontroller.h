/***************************************************************************
                          loadercontroller.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef LOADERCONTROLLER_H
#define LOADERCONTROLLER_H


// kakao
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KDocumentLoaderManager;
class KRecentFilesAction;
class KXmlGuiWindow;


class LoaderController : public KViewController
{
  Q_OBJECT

  public:
    LoaderController( KDocumentLoaderManager *loaderManager, KXmlGuiWindow *window );
    ~LoaderController();

  public: // KViewController API
    virtual void setView( KAbstractView *view );

  public Q_SLOTS:
    void load();
    void loadRecent( const KUrl &url );

    void onUrlUsed( const KUrl &url );

  protected:
    KDocumentLoaderManager *mLoaderManager;
    KXmlGuiWindow *mMainWindow;

    KRecentFilesAction *mOpenRecentAction;
};

#endif

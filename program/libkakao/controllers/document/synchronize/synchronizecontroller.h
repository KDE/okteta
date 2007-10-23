/***************************************************************************
                          synchronizecontroller.h  -  description
                             -------------------
    begin                : Sun Sep 2 2007
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


#ifndef KDOCUMENTSYNCHRONIZERTOOLET_H
#define KDOCUMENTSYNCHRONIZERTOOLET_H


// lib
#include <kabstractdocument.h>
#include <kviewcontroller.h>
// KDE
#include <KUrl>

class KDocumentSynchronizer;
class KAction;
class KXmlGuiWindow;
// class KAbstractDocument;
namespace KDE { namespace If {
class Storable;
} }

class SynchronizeController : public KViewController
{
  Q_OBJECT

  public:
    SynchronizeController( KDocumentSynchronizer *documentSynchronizer, KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void save();
    void saveAs();
    void reload();

  private Q_SLOTS:
    void onSynchronizationStateChange( KAbstractDocument::SynchronizationStates newStates );

  protected:
    KDocumentSynchronizer *mDocumentSynchronizer;
    KXmlGuiWindow *mMainWindow;
    KAbstractDocument *mDocument;
    KDE::If::Storable *mStoreControl;

    KAction *mSaveAction;
    KAction *mSaveAsAction;
    KAction *mReloadAction;
};

#endif

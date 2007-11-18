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

class KAbstractDocumentSynchronizer;
class KAbstractDocumentFileSystemSynchronizer;
class KAction;
class KXmlGuiWindow;

class SynchronizeController : public KViewController
{
  Q_OBJECT

  public:
    SynchronizeController( KXmlGuiWindow *window );

  public: // KToolet API
    virtual void setView( KAbstractView *view );

  protected Q_SLOTS:
    void save();
    void reload();

  private Q_SLOTS:
    void onSynchronizerChange( KAbstractDocumentSynchronizer *newSynchronizer );
    void onSynchronizationStateChange( KAbstractDocument::SynchronizationStates newStates );

  protected:
    KXmlGuiWindow *mMainWindow;
    KAbstractDocument *mDocument;
    KAbstractDocumentFileSystemSynchronizer *mSynchronizer;

    KAction *mSaveAction;
    KAction *mReloadAction;
};

#endif

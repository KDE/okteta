/***************************************************************************
                          shellwindow.h  -  description
                             -------------------
    begin                : Wed Oct 24 2007
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


#ifndef SHELLWINDOW_H
#define SHELLWINDOW_H


// kmvt
#include <kabstractdocument.h>
#include <kviewcontroller.h>
// KDE
#include <KXmlGuiWindow>
// Qt
#include <QtCore/QList>

class KDocumentManager;
class KAbstractView;
class KViewManager;
class TabbedViews;


class ShellWindow : public KXmlGuiWindow
{
   Q_OBJECT

  public:
    ShellWindow( KDocumentManager *documentManager, KViewManager *viewManager );
    virtual ~ShellWindow();

  public:
    void updateControllers( KAbstractView *view );

  protected: // KMainWindow API
    virtual bool queryClose();

  private Q_SLOTS:
    void onTitleChanged( const QString &newTitle );
    void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );
    void onViewFocusChanged( KAbstractView *view );

  protected:
    TabbedViews *mTabbedViews;
    // hack:
    // used to store a pointer to the current, so we can disconnect to its signals... well, not perfect
    KAbstractView *mCurrentView;

    KDocumentManager *mDocumentManager;
    KViewManager *mViewManager;
    QList<KViewController*> mControllers;
};

#endif

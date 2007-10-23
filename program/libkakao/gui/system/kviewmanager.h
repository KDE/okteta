/***************************************************************************
                          kviewmanager.h  -  description
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


#ifndef KVIEWMANAGER_H
#define KVIEWMANAGER_H

// lib
#include "kabstractview.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>

class KDocumentManager;
class KViewFactory;
class KXmlGuiWindow;


class KViewManager : public QObject
{
  Q_OBJECT

  public:
    explicit KViewManager( KDocumentManager *documentManager );
    virtual ~KViewManager();

  public:
    void setWindow( KXmlGuiWindow *window );
    void setViewFactory( KViewFactory *factory );

  public:
    QList<KAbstractView*> views() const;
    KAbstractView *viewByWidget( QWidget *widget ) const;

  public Q_SLOTS:
    void createViewFor( KAbstractDocument *document );
    /**
    * asks the manager to close and delete the view
    * may fail if the process if cancelled due to user input
    */
    void closeView( KAbstractView *view );

  Q_SIGNALS:
    // view was created and already added to the list
    void opened( KAbstractView *view );
    // view will be closed, already removed from list
    void closing( KAbstractView *view );

  protected:
    QList<KAbstractView*> mViewList;
    KViewFactory *mFactory;
    KDocumentManager *mDocumentManager;

    KXmlGuiWindow *mMainWindow;
};

#endif

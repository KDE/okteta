/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KVIEWMANAGER_H
#define KVIEWMANAGER_H

// lib
#include "kabstractview.h"
// Qt
#include <QtCore/QList>
#include <QtCore/QObject>

class KXmlGuiWindow;


namespace Kasten
{

class ModelCodecViewManager;
class KDocumentManager;
class KViewFactory;


class KASTENGUI_EXPORT KViewManager : public QObject
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
    KAbstractView* viewOfDocument( KAbstractDocument* document ) const;

  public:
    ModelCodecViewManager* codecViewManager();

  Q_SIGNALS:
    // view was created and already added to the list
    void opened( KAbstractView *view );
    // view will be closed, already removed from list
    void closing( KAbstractView *view );

  protected Q_SLOTS:
    void createViewFor( KAbstractDocument *document );
    void removeViewsFor( KAbstractDocument *document );
    /**
    * asks the manager to close and delete the view
    * may fail if the process if cancelled due to user input
    */
//     void closeView( KAbstractView *view );

  protected:
    QList<KAbstractView*> mViewList;
    KViewFactory *mFactory;
    KDocumentManager *mDocumentManager;

    KXmlGuiWindow *mMainWindow;

    // TODO: remove into own singleton
    ModelCodecViewManager* mCodecViewManager;
};


inline ModelCodecViewManager* KViewManager::codecViewManager() { return mCodecViewManager; }

}

#endif

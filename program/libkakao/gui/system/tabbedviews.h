/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef TABBEDVIEWS_H
#define TABBEDVIEWS_H


// lib
#include "abstractgroupedviews.h"
#include "kiviewfocusable.h"

class KTabWidget;
class KViewManager;
class QDragMoveEvent;
class QDropEvent;


class TabbedViews : public AbstractGroupedViews, public KDE::If::ViewFocusable
{
   Q_OBJECT
   Q_INTERFACES( KDE::If::ViewFocusable )

  public:
    explicit TabbedViews( KViewManager *viewManager );
    virtual ~TabbedViews();

  public: // KDE::If::ViewFocusable API
    virtual void setViewFocus( KAbstractView *view );
    virtual KAbstractView *viewFocus() const;

  public: // AbstractGroupedViews API
    virtual void addView( KAbstractView *view );
    virtual void removeView( KAbstractView *view );

    virtual QWidget *widget() const;


  Q_SIGNALS:
    virtual void viewFocusChanged( KAbstractView *view );
    void dragMove( const QDragMoveEvent* event, bool& accepted );
    void drop( QDropEvent* event );

  private Q_SLOTS:
    void onCurrentChanged( int index );
    void onCloseRequest( QWidget* widget );
    void onTitleChanged( const QString &newTitle );
//     void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );

  protected:
   KViewManager *mViewManager;
   KTabWidget *mTabWidget;
};

#endif

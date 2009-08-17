/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef MULTIVIEWAREAS_H
#define MULTIVIEWAREAS_H


// lib
#include "abstractgroupedviews.h"
#include "viewfocusable.h"
#include "viewareasplitable.h"
#include "toolinlineviewable.h"

class KTabWidget;
class QDragMoveEvent;
class QDropEvent;
class QSplitter;


namespace Kasten
{

class TabbedViews;


class KASTENGUI_EXPORT MultiViewAreas : public AbstractGroupedViews,
                                        public If::ViewFocusable,
//                                         public If::ToolInlineViewable,
                                        public If::ViewAreaSplitable
{
  Q_OBJECT
  Q_INTERFACES(
    Kasten::If::ViewFocusable
//     Kasten::If::ToolInlineViewable
    Kasten::If::ViewAreaSplitable
  )

  public:
    explicit MultiViewAreas();

    virtual ~MultiViewAreas();

  public: // AbstractViewArea API
    virtual void setFocus();
    virtual QWidget* widget() const;
    virtual bool hasFocus() const;

  public: // AbstractGroupedViews API
    virtual void addViews( const QList<AbstractView*>& views );
    virtual void removeViews( const QList<AbstractView*>& views );

    virtual QList<AbstractView*> viewList() const;
    virtual int viewCount() const;

  public: // If::ViewFocusable API
    virtual void setViewFocus( AbstractView* view );
    virtual AbstractView* viewFocus() const;

  public: // If::ToolInlineViewable API
//     virtual void addToolInlineView( AbstractToolInlineView* view );
//     virtual void setCurrentToolInlineView( AbstractToolInlineView* view );

  public: // If::ViewAreaSplitable API
    virtual AbstractViewArea* splitViewArea( AbstractViewArea* viewArea, Qt::Orientation orientation );
    virtual void closeViewArea( AbstractViewArea* viewArea );
    virtual void setViewAreaFocus( AbstractViewArea* viewArea );
    virtual AbstractViewArea* viewAreaFocus() const;
    virtual int viewAreasCount() const;

  Q_SIGNALS: // If::ViewFocusable API
    virtual void viewFocusChanged( Kasten::AbstractView* view );

  Q_SIGNALS: // If::ViewAreaSplitable API
    virtual void viewAreasAdded( const QList<Kasten::AbstractViewArea*>& viewAreas );
    virtual void viewAreasRemoved( const QList<Kasten::AbstractViewArea*>& viewAreas );
    virtual void viewAreaFocusChanged( Kasten::AbstractViewArea* viewArea );

  Q_SIGNALS:
    void dragMove( const QDragMoveEvent* event, bool& accepted );
    void drop( QDropEvent* event );

  protected:
    int indexOf( AbstractView* view ) const;

  protected Q_SLOTS:
    void onViewsRemoved();
    void onViewAreaFocusChanged( bool hasFocus );

  protected:
    QList<TabbedViews*> mViewAreaList;
    QSplitter* mMainSplitter;

    TabbedViews* mCurrentViewArea;
};

}
#endif

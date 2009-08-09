/*
    This file is part of the Kasten Framework, part of the KDE project.

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
#include "viewfocusable.h"
#include "toolinlineviewable.h"

class KTabWidget;
class QDragMoveEvent;
class QDropEvent;


namespace Kasten
{

class TabbedViewsBox;


class KASTENGUI_EXPORT TabbedViews : public AbstractGroupedViews, public If::ViewFocusable, public If::ToolInlineViewable
{
   Q_OBJECT
   Q_INTERFACES( Kasten::If::ViewFocusable Kasten::If::ToolInlineViewable )

  public:
    explicit TabbedViews();
    virtual ~TabbedViews();

  public: // If::ToolInlineViewable API
    virtual void addToolInlineView( AbstractToolInlineView* view );
    virtual void setCurrentToolInlineView( AbstractToolInlineView* view );

  public: // If::ViewFocusable API
    virtual void setViewFocus( AbstractView *view );
    virtual AbstractView *viewFocus() const;

  public: // AbstractGroupedViews API
    virtual void addViews( const QList<AbstractView*>& views );
    virtual void removeViews( const QList<AbstractView*>& views );

    virtual QWidget* widget() const;
    virtual QList<AbstractView*> viewList() const;

  Q_SIGNALS: // If::ViewFocusable API
    virtual void viewFocusChanged( Kasten::AbstractView* view );
  Q_SIGNALS:
    void dragMove( const QDragMoveEvent* event, bool& accepted );
    void drop( QDropEvent* event );

  private Q_SLOTS:
    void onCurrentChanged( int index );
    void onCloseRequest( QWidget* widget );
    void onTitleChanged( const QString &newTitle );
//     void onModifiedChanged( Kasten::AbstractDocument::SyncStates newStates );

  protected:
    int indexOf( AbstractView* view ) const;

  protected:
    TabbedViewsBox* mTabbedViewsBox;
    KTabWidget* mTabWidget;
    QList<AbstractToolInlineView*> mToolInlineViewList;
};

}
#endif

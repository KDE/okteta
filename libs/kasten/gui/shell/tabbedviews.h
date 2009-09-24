/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "toolinlineviewable.h"

class QDragMoveEvent;
class QDropEvent;


namespace Kasten
{

class ViewAreaBox;

class TabbedViewsPrivate;


class KASTENGUI_EXPORT TabbedViews : public AbstractGroupedViews,
                                     public If::ToolInlineViewable
{
  Q_OBJECT
  Q_INTERFACES(
    Kasten::If::ToolInlineViewable
  )

  protected:
    explicit TabbedViews( TabbedViewsPrivate* d );

  public:
    TabbedViews();

    virtual ~TabbedViews();

  public: // AbstractViewArea API
    virtual void setFocus();
    virtual QWidget* widget() const;
    virtual bool hasFocus() const;

  public: // AbstractGroupedViews API
    virtual void addViews( const QList<AbstractView*>& views );
    virtual void removeViews( const QList<AbstractView*>& views );
    virtual void setViewFocus( AbstractView* view );

    virtual QList<AbstractView*> viewList() const;
    virtual int viewCount() const;
    virtual AbstractView* viewFocus() const;

  public: // If::ToolInlineViewable API
    virtual void setCurrentToolInlineView( AbstractToolInlineView* view );

  public:
    int indexOf( AbstractView* view ) const;

  Q_SIGNALS:
    void dragMove( const QDragMoveEvent* event, bool& accepted );
    void drop( QDropEvent* event );

  protected:
    Q_PRIVATE_SLOT( d_func(), void onCurrentChanged( int index ) )
    Q_PRIVATE_SLOT( d_func(), void onCloseRequest( QWidget* widget ) )
    Q_PRIVATE_SLOT( d_func(), void onTitleChanged( const QString& title ) )
//     Q_PRIVATE_SLOT( d_func(), void onModifiedChanged( Kasten::AbstractDocument::SyncStates newStates ) )
    Q_PRIVATE_SLOT( d_func(), void onViewFocusChanged( bool focusChanged ) )

  protected:
    Q_DECLARE_PRIVATE( TabbedViews )
};

}
#endif

/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PARALLELVIEWS_H
#define PARALLELVIEWS_H


// lib
#include "abstractgroupedviews.h"
#include "viewfocusable.h"

class ViewManager;
class ParallelWidget;
class QHBoxLayout;


class ParallelViews : public AbstractGroupedViews, public If::ViewFocusable
{
   Q_OBJECT
   Q_INTERFACES( Kasten::If::ViewFocusable )

  public:
    explicit ParallelViews( ViewManager* viewManager );
    virtual ~ParallelViews();

  public: // If::ViewFocusable API
    virtual void setViewFocus( AbstractView* view );
    virtual AbstractView* viewFocus() const;

  public: // AbstractGroupedViews API
    virtual void addView( AbstractView* view );
    virtual void removeView( AbstractView* view );

    virtual QWidget* widget() const;


  Q_SIGNALS:
    virtual void viewFocusChanged( AbstractView* view );

  private Q_SLOTS:
    void onCurrentChanged( QWidget* widget );
    void onTitleChanged( const QString& newTitle );

  protected:
    ViewManager* mViewManager;
    ParallelWidget* mParallelWidget;
};

#endif

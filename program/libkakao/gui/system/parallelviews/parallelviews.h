/*
    This file is part of the Kakao Framework, part of the KDE project.

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
#include "kiviewfocusable.h"

class KViewManager;
class ParallelWidget;
class QHBoxLayout;


class ParallelViews : public AbstractGroupedViews, public KDE::If::ViewFocusable
{
   Q_OBJECT
   Q_INTERFACES( KDE::If::ViewFocusable )

  public:
    explicit ParallelViews( KViewManager* viewManager );
    virtual ~ParallelViews();

  public: // KDE::If::ViewFocusable API
    virtual void setViewFocus( KAbstractView* view );
    virtual KAbstractView* viewFocus() const;

  public: // AbstractGroupedViews API
    virtual void addView( KAbstractView* view );
    virtual void removeView( KAbstractView* view );

    virtual QWidget* widget() const;


  Q_SIGNALS:
    virtual void viewFocusChanged( KAbstractView* view );

  private Q_SLOTS:
    void onCurrentChanged( QWidget* widget );
    void onTitleChanged( const QString& newTitle );

  protected:
    KViewManager* mViewManager;
    ParallelWidget* mParallelWidget;
};

#endif

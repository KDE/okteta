/***************************************************************************
                          tabbedviews.h  -  description
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


#ifndef TABBEDVIEWS_H
#define TABBEDVIEWS_H


// lib
#include "abstractgroupedviews.h"
#include "kiviewfocusable.h"

class QTabWidget;
class KViewManager;


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

  private Q_SLOTS:
    void onCurrentChanged( int index );
    void onTitleChanged( const QString &newTitle );
//     void onModifiedChanged( KAbstractDocument::SynchronizationStates newStates );

  protected:
   KViewManager *mViewManager;
   QTabWidget *mTabWidget;
};

#endif

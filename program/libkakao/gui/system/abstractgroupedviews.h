/***************************************************************************
                          abstractgroupedviews.h  -  description
                             -------------------
    begin                : Wed Oct 24 2007
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


#ifndef ABSTRACTGROUPEDVIEWS_H
#define ABSTRACTGROUPEDVIEWS_H


// Qt
#include <QtCore/QObject>

class KAbstractView;
class QWidget;


class AbstractGroupedViews : public QObject
{
    Q_OBJECT
  public:
    AbstractGroupedViews();
    virtual ~AbstractGroupedViews();

  public Q_SLOTS: // set/action API to be implemented
    virtual void addView( KAbstractView *view ) = 0;
    virtual void removeView( KAbstractView *view ) = 0;

  public: // get API to be implemented
    virtual QWidget *widget() const = 0;

  Q_SIGNALS:
    // view was created and already added to the list
    void added( KAbstractView *view );
    // view will be removed, already removed from list
    void removing( KAbstractView *view );
};

#endif

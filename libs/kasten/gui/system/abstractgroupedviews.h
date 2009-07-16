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

#ifndef ABSTRACTGROUPEDVIEWS_H
#define ABSTRACTGROUPEDVIEWS_H

// lib
#include "kastengui_export.h"
// Qt
#include <QtCore/QObject>

class QWidget;


namespace Kasten
{

class AbstractView;


class KASTENGUI_EXPORT AbstractGroupedViews : public QObject
{
    Q_OBJECT
  public:
    AbstractGroupedViews();
    virtual ~AbstractGroupedViews();

  public Q_SLOTS: // set/action API to be implemented
    virtual void addView( Kasten::AbstractView* view ) = 0;
    virtual void removeView( Kasten::AbstractView* view ) = 0;

  public: // get API to be implemented
    virtual QWidget* widget() const = 0;
    // returns the list in the order of display
    virtual QList<AbstractView*> viewList() const = 0;

  Q_SIGNALS:
    // view was created and already added to the list
    void added( Kasten::AbstractView *view );
    // view will be removed, already removed from list
    void removing( Kasten::AbstractView *view );
    // closing the view is requested
    void closeRequest( Kasten::AbstractView* view );
};

}

#endif

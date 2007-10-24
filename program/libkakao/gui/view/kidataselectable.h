/***************************************************************************
                          kidataselectable.h  -  description
                             -------------------
    begin                : Mon Nov 13 2006
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


#ifndef KDE_IF_DATASELECTABLE_H
#define KDE_IF_DATASELECTABLE_H

// Qt
#include <QtCore/QtPlugin>

class QMimeData;

namespace KDE
{
namespace If
{

// TODO: this interface is strongly related to the selecteddatawriteable interface
class DataSelectable
{
  public:
    virtual ~DataSelectable();

  public: // set
    virtual void selectAllData( bool selectAll ) = 0;

  public: // get
    virtual bool hasSelectedData() const = 0;
    virtual QMimeData *copySelectedData() const = 0;

  public: // signal
    virtual void hasSelectedDataChanged( bool hasSelectedData ) = 0;
};

inline DataSelectable::~DataSelectable() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::DataSelectable, "org.kde.if.dataselectable/1.0" )

#endif

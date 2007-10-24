/***************************************************************************
                          kstorableinterface.h  -  description
                             -------------------
    begin                : Thu Nov 2 2006
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


#ifndef KDE_IF_STORABLE_H
#define KDE_IF_STORABLE_H

// Qt
#include <QtCore/QtPlugin>

namespace KDE
{
namespace If
{

/** Interface for storable objects
  * 
  * @author Friedrich W. H. Kossebau <kossebau@kde.org>
  */
class Storable
{
  public:
    virtual ~Storable();

  public: // set/action
    virtual bool load( const QString &fileName ) = 0;
    virtual bool save( const QString &fileName ) = 0;
    virtual void setUrl( const QString &url ) = 0;

  public: // get
    virtual QString url() const = 0;
    virtual QString localFilePath() const = 0;

  public: // signal
    virtual void urlChanged( const QString &url ) = 0;
};

inline Storable::~Storable() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::Storable, "org.kde.if.storable/1.0" )

#endif

/***************************************************************************
                          kzoominterface.h  -  description
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


#ifndef KDE_IF_ZOOMABLE_H
#define KDE_IF_ZOOMABLE_H


namespace KDE
{
namespace If
{

class Zoomable
{
  public:
    virtual ~Zoomable();

  public: // set/action
    virtual void setZoomLevel( double Level ) = 0;

  public: // get
    virtual double zoomLevel() const = 0;

  public: // signal
    virtual void zoomLevelChanged( double Level ) = 0;
};

inline Zoomable::~Zoomable() {}

}
}

Q_DECLARE_INTERFACE( KDE::If::Zoomable, "org.kde.if.zoomable/1.0" )

#endif

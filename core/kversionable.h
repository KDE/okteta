/***************************************************************************
                          kversionale.h  -  description
                             -------------------
    begin                : Fri Nov 30 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

#ifndef KHE_CORE_VERSIONABLE_H
#define KHE_CORE_VERSIONABLE_H

// Qt
#include <QtCore/QtPlugin>

class QString;

namespace KHECore
{

class Versionable
{
  public:
    virtual ~Versionable();

  public: // get
    virtual int versionIndex() const = 0;
    virtual int versionCount() const = 0;
    virtual QString versionDescription( int versionIndex ) const = 0;

  public: // set/action
    virtual void revertToVersionByIndex( int versionIndex ) = 0;

  public: // signal
    virtual void revertedToVersionIndex( int versionIndex ) = 0;
    virtual void headVersionDescriptionChanged( const QString &versionDescription ) = 0;
    virtual void headVersionChanged( int newHeadVersionIndex ) = 0;
};

inline Versionable::~Versionable() {}

}

Q_DECLARE_INTERFACE( KHECore::Versionable, "org.kde.khecore.versionable/1.0" )

#endif

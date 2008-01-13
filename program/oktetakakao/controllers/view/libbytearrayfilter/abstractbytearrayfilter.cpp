/***************************************************************************
                          abstractbytearrayfilter.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "abstractbytearrayfilter.h"

// Qt
#include <QtCore/QString>

class AbstractByteArrayFilter::Private
{
  public:
    explicit Private( const QString &name );
  public:
    QString name() const;

  protected:
    QString mName;
};

AbstractByteArrayFilter::Private::Private( const QString &name )
 : mName( name )
{}

inline QString AbstractByteArrayFilter::Private::name() const { return mName; }


AbstractByteArrayFilter::AbstractByteArrayFilter( const QString &name )
 : d( new Private(name) )
{}

QString AbstractByteArrayFilter::name() const { return d->name(); }

AbstractByteArrayFilter::~AbstractByteArrayFilter()
{
    delete d;
}

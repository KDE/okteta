/***************************************************************************
                          kviewfactory.h  -  description
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


#ifndef KVIEWFACTORY_H
#define KVIEWFACTORY_H


class KAbstractDocument;
class KAbstractView;


class KViewFactory
{
  public:
    virtual ~KViewFactory();

  public:
    virtual KAbstractView *createViewFor( KAbstractDocument *Document ) = 0;
};

inline KViewFactory::~KViewFactory() {}

#endif

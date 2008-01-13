/***************************************************************************
                          nobytearrayfilterparameterset.h  -  description
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


#ifndef NOBYTEARRAYFILTERPARAMETERSET_H
#define NOBYTEARRAYFILTERPARAMETERSET_H

// lib
#include "abstractbytearrayfilterparameterset.h"

class NoByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
  public:
    NoByteArrayFilterParameterSet();
    virtual ~NoByteArrayFilterParameterSet();
};

#endif

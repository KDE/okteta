/***************************************************************************
                          reversebytearrayfilterparameterset.h  -  description
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


#ifndef REVERSEBYTEARRAYFILTERPARAMETERSET_H
#define REVERSEBYTEARRAYFILTERPARAMETERSET_H

// lib
#include "abstractbytearrayfilterparameterset.h"


class ReverseByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
  public:
    ReverseByteArrayFilterParameterSet();
    virtual ~ReverseByteArrayFilterParameterSet();

  public:
    void setInvertsBits( bool invertsBits );

  public:
    bool invertsBits() const;

  protected:
    bool mInvertsBits;
};

#endif

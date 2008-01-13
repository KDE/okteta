/***************************************************************************
                          rotatebytearrayfilterparameterset.h  -  description
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


#ifndef ROTATEBYTEARRAYFILTERPARAMETERSET_H
#define ROTATEBYTEARRAYFILTERPARAMETERSET_H

// lib
#include "abstractbytearrayfilterparameterset.h"

class RotateByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
  public:
    RotateByteArrayFilterParameterSet();
    virtual ~RotateByteArrayFilterParameterSet();

  public:
    void setGroupSize( int groupSize );
    void setMoveBitWidth( int moveBitWidth );

  public:
    int groupSize() const;
    int moveBitWidth() const;

  protected:
    int mGroupSize;
    int mMoveBitWidth;
};

#endif

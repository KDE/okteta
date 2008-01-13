/***************************************************************************
                          rotatebytearrayfilterparameterset.cpp  -  description
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


#include "rotatebytearrayfilterparameterset.h"


RotateByteArrayFilterParameterSet::RotateByteArrayFilterParameterSet()
{}

int RotateByteArrayFilterParameterSet::groupSize() const { return mGroupSize; }
int RotateByteArrayFilterParameterSet::moveBitWidth() const { return mMoveBitWidth; }

void RotateByteArrayFilterParameterSet::setGroupSize( int groupSize ) { mGroupSize = groupSize; }
void RotateByteArrayFilterParameterSet::setMoveBitWidth( int moveBitWidth ) { mMoveBitWidth = moveBitWidth; }

RotateByteArrayFilterParameterSet::~RotateByteArrayFilterParameterSet()
{
}

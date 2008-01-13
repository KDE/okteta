/***************************************************************************
                          reversebytearrayfilterparameterset.cpp  -  description
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


#include "reversebytearrayfilterparameterset.h"


ReverseByteArrayFilterParameterSet::ReverseByteArrayFilterParameterSet() {}

bool ReverseByteArrayFilterParameterSet::invertsBits() const { return mInvertsBits; }

void ReverseByteArrayFilterParameterSet::setInvertsBits( bool invertsBits ) { mInvertsBits = invertsBits; }

ReverseByteArrayFilterParameterSet::~ReverseByteArrayFilterParameterSet() {}

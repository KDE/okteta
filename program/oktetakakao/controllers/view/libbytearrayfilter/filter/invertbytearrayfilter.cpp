/***************************************************************************
                          invertbytearrayfilter.cpp  -  description
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


#include "invertbytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>


InvertByteArrayFilter::InvertByteArrayFilter()
 : AbstractByteArrayFilter( i18n("INVERT data") )
{}

AbstractByteArrayFilterParameterSet *InvertByteArrayFilter::parameterSet() { return &mNoParameterSet; }

bool InvertByteArrayFilter::filter( char *result,
                                    KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    int r = 0;
    int m = section.start();
    while( m <= section.end() )
        result[r++] = ~model->datum( m++ );

    return true;
}

InvertByteArrayFilter::~InvertByteArrayFilter() {}

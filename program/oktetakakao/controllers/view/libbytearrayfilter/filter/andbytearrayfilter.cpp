/***************************************************************************
                          andbytearrayfilter.cpp  -  description
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


#include "andbytearrayfilter.h"

// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>


AndByteArrayFilter::AndByteArrayFilter()
 : AbstractByteArrayFilter( i18n("operand AND data") )
{}

AbstractByteArrayFilterParameterSet *AndByteArrayFilter::parameterSet() { return &mParameterSet; }

bool AndByteArrayFilter::filter( char *result,
                                 KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const
{
    const QByteArray operand = mParameterSet.operand();
    const int operandSize = operand.size();

    if( mParameterSet.alignAtEnd() )
    {
        int r = section.width();
        int m = section.end() + 1;

        while( m > 0 )
        {
            int o = operandSize;
            while( m > 0 && o > 0 )
                result[(r--)-1] = model->datum( (m--)-1 ) & operand[(o--)-1];
        }
    }
    else
    {
        int r = 0;
        int m = section.start();

        while( m <= section.end() )
        {
            int o = 0;
            while( m <= section.end() && o < operandSize )
                result[r++] = model->datum( m++ ) & operand[o++];
        }
    }

    return true;
}

AndByteArrayFilter::~AndByteArrayFilter()
{
}

/***************************************************************************
                          nobytearrayfilterparametersetedit.cpp  -  description
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


#include "nobytearrayfilterparametersetedit.h"


NoByteArrayFilterParameterSetEdit::NoByteArrayFilterParameterSetEdit( QWidget *parent )
 : AbstractByteArrayFilterParameterSetEdit( parent )
{}

void NoByteArrayFilterParameterSetEdit::setValues( const AbstractByteArrayFilterParameterSet *parameterSet )
{
    Q_UNUSED( parameterSet );
}

void NoByteArrayFilterParameterSetEdit::getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const
{
    Q_UNUSED( parameterSet );
}

NoByteArrayFilterParameterSetEdit::~NoByteArrayFilterParameterSetEdit()
{}

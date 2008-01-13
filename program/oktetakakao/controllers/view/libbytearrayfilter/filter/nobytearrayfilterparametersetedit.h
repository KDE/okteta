/***************************************************************************
                          nobytearrayfilterparametersetedit.h  -  description
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


#ifndef NOBYTEARRAYFILTERPARAMETERSETEDIT_H
#define NOBYTEARRAYFILTERPARAMETERSETEDIT_H


// lib
#include "abstractbytearrayfilterparametersetedit.h"

class NoByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
  public:
    explicit NoByteArrayFilterParameterSetEdit( QWidget *parent = 0 );
    virtual ~NoByteArrayFilterParameterSetEdit();

  public: // AbstractByteArrayFilterParameterSetEdit API
    virtual void setValues( const AbstractByteArrayFilterParameterSet *parameterSet );
    virtual void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const;

};

#endif

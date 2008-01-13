/***************************************************************************
                          reversebytearrayfilterparametersetedit.h  -  description
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


#ifndef REVERSEBYTEARRAYFILTERPARAMETERSETEDIT_H
#define REVERSEBYTEARRAYFILTERPARAMETERSETEDIT_H


// lib
#include "abstractbytearrayfilterparametersetedit.h"

class QCheckBox;

class ReverseByteArrayFilterParameterSetEdit : public AbstractByteArrayFilterParameterSetEdit
{
  Q_OBJECT

  public:
    explicit ReverseByteArrayFilterParameterSetEdit( QWidget *parent = 0 );
    virtual ~ReverseByteArrayFilterParameterSetEdit();

  public: // AbstractByteArrayFilterParameterSetEdit API
    virtual void setValues( const AbstractByteArrayFilterParameterSet *parameterSet );
    virtual void getParameterSet( AbstractByteArrayFilterParameterSet *parameterSet ) const;

  protected:
    QCheckBox *mInvertsBitsCheckBox;
};

#endif

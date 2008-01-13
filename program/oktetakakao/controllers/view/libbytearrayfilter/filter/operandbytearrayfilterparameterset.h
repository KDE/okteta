/***************************************************************************
                          operandbytearrayfilterparameterset.h  -  description
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


#ifndef OPERANDBYTEARRAYFILTERPARAMETERSET_H
#define OPERANDBYTEARRAYFILTERPARAMETERSET_H

// lib
#include "abstractbytearrayfilterparameterset.h"
// Qt
#include <QtCore/QByteArray>


class OperandByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
  public:
    OperandByteArrayFilterParameterSet();
    virtual ~OperandByteArrayFilterParameterSet();

  public:
    void setOperand( const QByteArray &operand );
    void setOperandFormat( int operandFormat );
    void setAlignAtEnd( bool alignAtEnd );

  public:
    QByteArray operand() const;
    int operandFormat() const;
    bool alignAtEnd() const;

  protected:
    QByteArray mOperand;
    int mOperandFormat;
    bool mAlignAtEnd;
};

#endif

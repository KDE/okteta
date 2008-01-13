/***************************************************************************
                          operandbytearrayfilterparameterset.cpp  -  description
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


#include "operandbytearrayfilterparameterset.h"


OperandByteArrayFilterParameterSet::OperandByteArrayFilterParameterSet()
{}


QByteArray OperandByteArrayFilterParameterSet::operand() const { return mOperand; }
int OperandByteArrayFilterParameterSet::operandFormat() const { return mOperandFormat; }
bool OperandByteArrayFilterParameterSet::alignAtEnd() const { return mAlignAtEnd; }

void OperandByteArrayFilterParameterSet::setOperand( const QByteArray &operand ) { mOperand = operand; }
void OperandByteArrayFilterParameterSet::setOperandFormat( int operandFormat ) { mOperandFormat = operandFormat; }
void OperandByteArrayFilterParameterSet::setAlignAtEnd( bool alignAtEnd ) { mAlignAtEnd = alignAtEnd; }

OperandByteArrayFilterParameterSet::~OperandByteArrayFilterParameterSet()
{
}

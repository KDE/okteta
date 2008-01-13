/***************************************************************************
                          xorbytearrayfilter.h  -  description
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


#ifndef XORBYTEARRAYFILTER_H
#define XORBYTEARRAYFILTER_H


////
#include "operandbytearrayfilterparameterset.h"
//
#include <abstractbytearrayfilter.h>

class XOrByteArrayFilter : public AbstractByteArrayFilter
{
  public:
    XOrByteArrayFilter();
    virtual ~XOrByteArrayFilter();

  public: // AbstractByteArrayFilter API
    virtual bool filter( char *result, KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const;
    virtual AbstractByteArrayFilterParameterSet *parameterSet();

  protected:
    OperandByteArrayFilterParameterSet mParameterSet;
};


#endif

/***************************************************************************
                          rotatebytearrayfilter.h  -  description
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


#ifndef ROTATEBYTEARRAYFILTER_H
#define ROTATEBYTEARRAYFILTER_H


//
#include "rotatebytearrayfilterparameterset.h"
//
#include <abstractbytearrayfilter.h>


class RotateByteArrayFilter : public AbstractByteArrayFilter
{
  public:
    RotateByteArrayFilter();
    virtual ~RotateByteArrayFilter();

  public: // AbstractByteArrayFilter API
    virtual bool filter( char *result, KHECore::KAbstractByteArrayModel *model, const KHE::KSection &section ) const;
    virtual AbstractByteArrayFilterParameterSet *parameterSet();

  protected:
    RotateByteArrayFilterParameterSet mParameterSet;
};

#endif

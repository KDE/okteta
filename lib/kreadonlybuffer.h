/***************************************************************************
                          kreadonlybuffer.h  -  description
                             -------------------
    begin                : Mit Mai 14 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/

 
#ifndef KHE_KREADONLYBUFFER_H
#define KHE_KREADONLYBUFFER_H

#include "kdatabuffer.h"

namespace KHE
{

/**
  *@author Friedrich W. H. Kossebau
  */

class KReadOnlyBuffer : public KDataBuffer
{
  public:
    KReadOnlyBuffer() {}
    ~KReadOnlyBuffer() {}

  public: // KDataBuffer API
    /** is the buffer changeable ?*/
    virtual bool isReadOnly() const   { return true; }
    /** has the buffer been modified? */
    virtual bool isModified() const   { return false; }
};

}

#endif

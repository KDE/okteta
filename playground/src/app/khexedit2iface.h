/***************************************************************************
                          khexedit2iface.h  -  description
                             -------------------
    begin                : Tue Jul 29 2003
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


#ifndef _KHEXEDIT2IFACE_H_
#define _KHEXEDIT2IFACE_H_

#include <dcopobject.h>

class KHexEdit2Iface : virtual public DCOPObject
{
  K_DCOP
  public:

  k_dcop:
    virtual void openURL(QString url) = 0;
};

#endif // _KHEXEDIT2IFACE_H_

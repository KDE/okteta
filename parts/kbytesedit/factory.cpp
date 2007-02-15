/***************************************************************************
                          factory.cpp  -  description
                             -------------------
    begin                : Fri Aug 29 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// kde specific
#include <kgenericfactory.h>
// component specific
#include "kbytearrayedit.h"


typedef K_TYPELIST_1( KByteArrayEdit ) Product;
K_EXPORT_COMPONENT_FACTORY( libkbytearrayedit, KGenericFactory<Product>( "kbytearrayedit" ) )

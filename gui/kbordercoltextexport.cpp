/***************************************************************************
                          kbordercoltextexport.cpp  -  description
                             -------------------
    begin                : Sam Aug 30 2003
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


#include "kbordercoltextexport.h"

// Qt
#include <QtCore/QString>


namespace KHEUI {

static const uint BorderColumnTEWidth = 3;


int KBorderColTextExport::charsPerLine() const
{
  return BorderColumnTEWidth;
}

void KBorderColTextExport::printFirstLine( QString *T, int /*Line*/ ) const
{
  print( T );
}

void KBorderColTextExport::printNextLine( QString *T ) const
{
  print( T );
}

void KBorderColTextExport::print( QString *T ) const
{
  T->append( " | " );
}

}

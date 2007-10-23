/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Thu Jun 1 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


// program
#include "program.h"


int main( int argc, char *argv[] )
{
  OktetaProgram program( argc, argv );

  int result = program.execute();

  return result;
}

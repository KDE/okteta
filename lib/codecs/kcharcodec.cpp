/***************************************************************************
                          kcharcodec.cpp  -  description
                             -------------------
    begin                : Do Nov 25 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/



#include "kcharcodec.h"
#include "ktextcharcodec.h"
#include "kebcdic1047charcodec.h"

using namespace KHE;

/*QStringList KCharCodec::descriptiveCodecNames()
{
}
*/


KCharCodec *KCharCodec::createCodec( KEncoding C )
{
  KCharCodec *Codec;
  switch( C )
  {
    case ISO8859_1Encoding: Codec = KTextCharCodec::create("ISO 8859-1"); break;
    case EBCDIC1047Encoding: Codec = KEBCDIC1047CharCodec::create(); break;
    case LocalEncoding: // trigger default
    default: Codec = 0;
  }

  // ensure at least a codec
  if( Codec == 0 )
    Codec = KTextCharCodec::createLocal();

  return Codec;
}

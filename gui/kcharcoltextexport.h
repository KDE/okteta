/***************************************************************************
                          kcharcoltextexport.h  -  description
                             -------------------
    begin                : Wed Sep 3 2003
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


#ifndef KHE_UI_KCHARCOLTEXTEXPORT_H
#define KHE_UI_KCHARCOLTEXTEXPORT_H

// lib specific
#include "khe.h"
#include "kdatacoltextexport.h"

namespace KHECore {
class KCharCodec;
}

namespace KHEUI
{

class KCharColumn;

class KCharColTextExport : public KDataColTextExport
{
  public:
    KCharColTextExport( const KCharColumn* BF, const char *D, const KCoordRange &CR, const QString &CodecName );
    virtual ~KCharColTextExport();

  protected: //API
    virtual void print( QString *T ) const;

  protected:
    KHECore::KCharCodec *CharCodec;
    QChar SubstituteChar;
    QChar UndefinedChar;
};

}

#endif

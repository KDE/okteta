/***************************************************************************
                          kcharcodec.h  -  description
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


#ifndef KHE_KCHARCODEC_H
#define KHE_KCHARCODEC_H

// qt specific
#include <qstringlist.h>
// lib specific
#include "khe.h"
#include "khechar.h"
#include "khexedit_export.h"


namespace KHE
{

class KHEXEDIT_EXPORT KCharCodec
{
  public:
    /** */
    static KCharCodec* createCodec( KEncoding E );
    /** */
    static KCharCodec* createCodec( const QString &Name );

    static const QStringList &codecNames();

  public: // API to be implemented
    virtual KHEChar decode( char Byte ) const = 0;
    virtual bool encode( char *D, const QChar &C ) const = 0;
    virtual const QString& name() const = 0;

  protected:
    /** */
    static QStringList CodecNames;
};

}

#endif


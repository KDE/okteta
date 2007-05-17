/***************************************************************************
                          kcharcodec.h  -  description
                             -------------------
    begin                : Do Nov 25 2004
    copyright            : (C) 2004 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


#ifndef KHE_CORE_KCHARCODEC_H
#define KHE_CORE_KCHARCODEC_H


// lib
#include "khe.h"
#include "khe_export.h"

class QStringList;
class QChar;


namespace KHECore
{

class KChar;


class KHECORE_EXPORT KCharCodec
{
  public:
    virtual ~KCharCodec() {}

  public:
    /** */
    static KCharCodec* createCodec( KEncoding E );
    /** */
    static KCharCodec* createCodec( const QString &Name );

    static const QStringList &codecNames();

  public: // API to be implemented
    virtual KChar decode( char Byte ) const = 0;
    virtual bool encode( char *D, const QChar &C ) const = 0;
    virtual bool canEncode( const QChar &C ) const = 0;
    virtual const QString& name() const = 0;
};

}

#endif


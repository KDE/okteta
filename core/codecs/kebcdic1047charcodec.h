/***************************************************************************
                          kebcdic1047charcodec.h  -  description
                             -------------------
    begin                : Sa Nov 27 2004
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


#ifndef KHE_CORE_KEBCDIC1047CHARCODEC_H
#define KHE_CORE_KEBCDIC1047CHARCODEC_H

// lib specific
#include "kcharcodec.h"

namespace KHECore
{

//
class KEBCDIC1047CharCodec : public KCharCodec
{
  protected:
    KEBCDIC1047CharCodec();

  public: // KCharCodec API
    virtual KChar decode( char Byte ) const;
    virtual bool encode( char *D, const QChar &C ) const;
    virtual bool canEncode( const QChar &C ) const;
    virtual const QString& name() const;

  public:
    static KEBCDIC1047CharCodec *create();
    static const QString& codecName();
};


inline KEBCDIC1047CharCodec::KEBCDIC1047CharCodec() {}

inline KEBCDIC1047CharCodec *KEBCDIC1047CharCodec::create() { return new KEBCDIC1047CharCodec(); }

}

#endif

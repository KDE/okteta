/***************************************************************************
                          kebcdic1047charcodec.h  -  description
                             -------------------
    begin                : Sa Nov 27 2004
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


#ifndef KHE_KEBCDIC1047CHARCODEC_H
#define KHE_KEBCDIC1047CHARCODEC_H


#include "kcharcodec.h"

namespace KHE
{

//
class KEBCDIC1047CharCodec : public KCharCodec
{
  protected:
    KEBCDIC1047CharCodec();

  public: // KCharCodec API
    virtual KHEChar decode( char Byte ) const;
    virtual bool encode( char *D, const QChar &C ) const;
    virtual const QString& name() const;

  public:
    static KEBCDIC1047CharCodec *create();
    static const QString& codecName();
};


inline KEBCDIC1047CharCodec::KEBCDIC1047CharCodec() {}

inline KEBCDIC1047CharCodec *KEBCDIC1047CharCodec::create() { return new KEBCDIC1047CharCodec(); }

}

#endif

/***************************************************************************
                          ktextcharcodec.h  -  description
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


#ifndef KHE_KTEXTCHARCODEC_H
#define KHE_KTEXTCHARCODEC_H


#include "kcharcodec.h"

class QTextCodec;
class QTextDecoder;
class QTextEncoder;

namespace KHE
{

// used by all codecs with full char coping, i.e. there are no undefined chars
class KTextCharCodec : public KCharCodec
{
  public:
    static KTextCharCodec *createCodec( const QString &CodeName );
    static KTextCharCodec *createCodec( KEncoding C );
    static KTextCharCodec *createLocalCodec();

    static const QStringList &codecNames();

  protected:
    KTextCharCodec( QTextCodec *C );
  public:
    virtual ~KTextCharCodec();

  public: // KCharCodec API
    virtual bool encode( char *D, const QChar &C ) const;
    virtual KHEChar decode( char Byte ) const;
    virtual const QString& name() const;


  protected:
    QTextCodec *Codec;
    /** decodes the chars to unicode */
    QTextDecoder *Decoder;
    /** encodes the chars from unicode */
    QTextEncoder *Encoder;
    /** */
    mutable QString Name;

    static QStringList CodecNames;
};

}

#endif

/***************************************************************************
                          kbytearraytextstreamencoder.h  -  description
                             -------------------
    begin                : Fri Nov 23 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearraytextstreamencoder.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


TextStreamEncoderSettings::TextStreamEncoderSettings()
 : codecName(), undefinedChar('?'), substituteChar( '.' )
{}

KByteArrayTextStreamEncoder::KByteArrayTextStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18n("Plain Text"), QLatin1String("text/plain") )
{}


bool KByteArrayTextStreamEncoder::encodeDataToStream( QIODevice *device,
                                                      const char *data, int size )
{
    bool success = true;

    QTextStream textStream( device );

    KHECore::KCharCodec *charCodec = KHECore::KCharCodec::createCodec( mSettings.codecName );
    static const QChar tabChar( '\t' );
    static const QChar returnChar( '\n' );

    for( int i=0; i<size; ++i )
    {
        KHECore::KChar byteChar = charCodec->decode( data[i] );

        const QChar streamChar = byteChar.isUndefined() ?      KHECore::KChar(mSettings.undefinedChar) :
                                 (!byteChar.isPrint()
                                  && byteChar != tabChar
                                  && byteChar != returnChar) ? KHECore::KChar(mSettings.substituteChar) :
                                                               byteChar;
        textStream << streamChar;
    }
    // clean up
    delete charCodec;
    return success;
}

KByteArrayTextStreamEncoder::~KByteArrayTextStreamEncoder() {}

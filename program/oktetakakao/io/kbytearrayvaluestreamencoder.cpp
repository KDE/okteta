/***************************************************************************
                          kbytearrayvaluestreamencoder.h  -  description
                             -------------------
    begin                : Sat Nov 24 2007
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


#include "kbytearrayvaluestreamencoder.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
// Okteta core
#include <kabstractbytearraymodel.h>
#include <kbytecodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


ValueStreamEncoderSettings::ValueStreamEncoderSettings()
 : coding( KHECore::HexadecimalCoding), separation( QLatin1String(" ") )
{}

KByteArrayValueStreamEncoder::KByteArrayValueStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18n("Values"), QLatin1String("text/plain") )
{}


bool KByteArrayValueStreamEncoder::encodeDataToStream( QIODevice *device,
                                                       const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                       const KHE::KSection &section )
{
    bool success = true;

    QTextStream textStream( device );

    KHECore::KByteCodec *byteCodec = KHECore::KByteCodec::createCodec( mSettings.coding );

    // prepare 
    QString valueString;
    valueString.resize( byteCodec->encodingWidth() );

    for( int i=section.start(); i<=section.end(); ++i )
    {
        if( i > 0 )
            textStream << mSettings.separation;

        byteCodec->encode( valueString, 0, byteArrayModel->datum(i) );

        textStream << valueString;
    }
    // clean up
    delete byteCodec;

    return success;
}

KByteArrayValueStreamEncoder::~KByteArrayValueStreamEncoder() {}

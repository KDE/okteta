/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "kbytearraytextstreamencoder.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
// Okteta core
#include <kabstractbytearraymodel.h>
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
 : KAbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Plain Text"), QLatin1String("text/plain") )
{}


bool KByteArrayTextStreamEncoder::encodeDataToStream( QIODevice *device,
                                                      const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                      const KHE::KSection &section )
{
    bool success = true;

    QTextStream textStream( device );

    KHECore::KCharCodec *charCodec = KHECore::KCharCodec::createCodec( mSettings.codecName );
    static const QChar tabChar( '\t' );
    static const QChar returnChar( '\n' );

    for( int i=section.start(); i<=section.end(); ++i )
    {
        KHECore::KChar byteChar = charCodec->decode( byteArrayModel->datum(i) );

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

/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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
#include <kbytearraydisplay.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <khechar.h>
#include <charcodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten
{

TextStreamEncoderSettings::TextStreamEncoderSettings()
 : codecName(), undefinedChar('?'), substituteChar( '.' )
{}

KByteArrayTextStreamEncoder::KByteArrayTextStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Plain Text"), QLatin1String("text/plain") )
{}


bool KByteArrayTextStreamEncoder::encodeDataToStream( QIODevice *device,
                                                      const KByteArrayDisplay* byteArrayView,
                                                      const KHECore::AbstractByteArrayModel *byteArrayModel,
                                                      const KHE::Section &section )
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();

    // encode
    QTextStream textStream( device );

    KHECore::CharCodec *charCodec = KHECore::CharCodec::createCodec( mSettings.codecName );
    const QChar tabChar( '\t' );
    const QChar returnChar( '\n' );

    for( int i=section.start(); i<=section.end(); ++i )
    {
        const KHECore::KChar byteChar = charCodec->decode( byteArrayModel->datum(i) );

        const QChar streamChar = byteChar.isUndefined() ?      mSettings.undefinedChar :
                                 (!byteChar.isPrint()
                                  || byteChar == tabChar
                                  || byteChar == returnChar) ? mSettings.substituteChar :
                                                               (QChar)byteChar;
        textStream << streamChar;
    }
    // clean up
    delete charCodec;
    return success;
}

KByteArrayTextStreamEncoder::~KByteArrayTextStreamEncoder() {}

}

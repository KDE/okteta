/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "bytearraycharsstreamencoder.h"

// lib
#include <bytearrayview.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <character.h>
#include <charcodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

CharsStreamEncoderSettings::CharsStreamEncoderSettings()
  : codecName(),
    undefinedChar( QLatin1Char('?') ),
    substituteChar( QLatin1Char('.') )
{}

ByteArrayCharsStreamEncoder::ByteArrayCharsStreamEncoder()
 : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Characters"), QString::fromLatin1("text/plain") )
{}


bool ByteArrayCharsStreamEncoder::encodeDataToStream( QIODevice* device,
                                                      const ByteArrayView* byteArrayView,
                                                      const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                      const Okteta::AddressRange& range )
{
    bool success = true;

    // settings
    mSettings.codecName = byteArrayView->charCodingName();
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();

    // encode
    QTextStream textStream( device );

    Okteta::CharCodec *charCodec = Okteta::CharCodec::createCodec( mSettings.codecName );
    const QChar tabChar = QLatin1Char( '\t' );
    const QChar returnChar = QLatin1Char( '\n' );

    for( Okteta::Address i=range.start(); i<=range.end(); ++i )
    {
        const Okteta::Character byteChar = charCodec->decode( byteArrayModel->byte(i) );

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

ByteArrayCharsStreamEncoder::~ByteArrayCharsStreamEncoder() {}

}

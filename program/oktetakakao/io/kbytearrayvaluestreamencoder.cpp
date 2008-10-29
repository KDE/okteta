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

#include "kbytearrayvaluestreamencoder.h"

// lib
#include <kbytearraydisplay.h>
// Okteta core
#include <kabstractbytearraymodel.h>
#include <valuecodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


ValueStreamEncoderSettings::ValueStreamEncoderSettings()
 : valueCoding( KHECore::HexadecimalCoding), separation( QLatin1String(" ") )
{}

KByteArrayValueStreamEncoder::KByteArrayValueStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18nc("name of the encoding target","Values"), QLatin1String("text/plain") )
{}


bool KByteArrayValueStreamEncoder::encodeDataToStream( QIODevice *device,
                                                       const KByteArrayDisplay* byteArrayView,
                                                       const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                       const KHE::KSection &section )
{
    bool success = true;

    // settings
    mSettings.undefinedChar = byteArrayView->undefinedChar();
    mSettings.substituteChar = byteArrayView->substituteChar();
    mSettings.valueCoding = (KHECore::ValueCoding)byteArrayView->valueCoding();

    // encode
    QTextStream textStream( device );

    KHECore::ValueCodec *valueCodec = KHECore::ValueCodec::createCodec( mSettings.valueCoding );

    // prepare 
    QString valueString;
    valueString.resize( valueCodec->encodingWidth() );

    for( int i=section.start(); i<=section.end(); ++i )
    {
        if( i > 0 )
            textStream << mSettings.separation;

        valueCodec->encode( valueString, 0, byteArrayModel->datum(i) );

        textStream << valueString;
    }
    // clean up
    delete valueCodec;

    return success;
}

KByteArrayValueStreamEncoder::~KByteArrayValueStreamEncoder() {}

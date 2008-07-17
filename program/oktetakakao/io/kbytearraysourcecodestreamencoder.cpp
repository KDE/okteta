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

#include "kbytearraysourcecodestreamencoder.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
// Okteta core
#include <kabstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>
// C
#include <stdlib.h>


static const char *PrimitiveDataTypeName[] =
{
    "char",
    "unsigned char",
    "short",
    "unsigned short",
    "int",
    "unsigned int",
    "float",
    "double"
};

static const int SizeOfPrimitiveDataType[] =
{
    sizeof(char),
    sizeof(unsigned char),
    sizeof(short),
    sizeof(unsigned short),
    sizeof(int),
    sizeof(unsigned int),
    sizeof(float),
    sizeof(double)
};

static const int NoOfPrimitiveDataTypes = 8;




SourceCodeStreamEncoderSettings::SourceCodeStreamEncoderSettings()
 : variableName( QLatin1String("array") ), dataType( CharType ), elementsPerLine( 8 ), unsignedAsHexadecimal( false )
{}

KByteArraySourceCodeStreamEncoder::KByteArraySourceCodeStreamEncoder()
 : KAbstractByteArrayStreamEncoder( i18nc("name of the encoding target","C array"), QLatin1String("text/x-csrc") )
{}


bool KByteArraySourceCodeStreamEncoder::encodeDataToStream( QIODevice *device,
                                                            const KHEUI::KByteArrayView *byteArrayView,
                                                            const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                                            const KHE::KSection &section )
{
Q_UNUSED( byteArrayView )

    bool success = true;

    // settings

    // encode
    QTextStream textStream( device );

    const int size = section.width();
    const int dataTypeSize = SizeOfPrimitiveDataType[mSettings.dataType];
    const int sizeOfArray = (size+dataTypeSize-1) / dataTypeSize;

    textStream << QLatin1String(PrimitiveDataTypeName[mSettings.dataType]) << " "
               << mSettings.variableName << "[" << sizeOfArray << "] = {" << endl;

    int elementAddedOnLine = 0;
    for( int i=0; i<size; i+=dataTypeSize )
    {
        static char buffer[12];
        printFormatted( buffer, byteArrayModel, i, size-i );
        textStream << buffer;
        if( i + dataTypeSize < size )
            textStream << ",";

        if( ++elementAddedOnLine >= mSettings.elementsPerLine )
        {
            textStream << endl;
            elementAddedOnLine = 0;
        }
    }

    textStream << "};" << endl;

    return success;
}


void KByteArraySourceCodeStreamEncoder::printFormatted( char *buffer,
                                                        const KHECore::KAbstractByteArrayModel *byteArrayModel, int pos,
                                                        unsigned int dataSize ) const
{
    switch( mSettings.dataType )
    {
    case SourceCodeStreamEncoderSettings::CharType:
    {
        char e = 0;
        byteArrayModel->copyTo( &e, pos, qMin<size_t>(sizeof(e),dataSize) );
        sprintf( buffer, "%d", e );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedCharType:
    {
        unsigned char e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        if( mSettings.unsignedAsHexadecimal )
        {
        sprintf( buffer, "0x%02x", e );
        }
        else
        {
        sprintf( buffer, "%u", e );
        }
        break;
    }
    case SourceCodeStreamEncoderSettings::ShortType:
    {
        short e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        sprintf( buffer, "%d", e );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedShortType:
    {
        unsigned short e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        if( mSettings.unsignedAsHexadecimal )
        {
        sprintf( buffer, "0x%04x", e );
        }
        else
        {
        sprintf( buffer, "%u", e );
        }
        break;
    }
    case SourceCodeStreamEncoderSettings::IntegerType:
    {
        int e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        sprintf( buffer, "%u", e );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedIntegerType:
    {
        unsigned int e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        if( mSettings.unsignedAsHexadecimal )
        {
        sprintf( buffer, "0x%08x", e );
        }
        else
        {
        sprintf( buffer, "%u", e );
        }
        break;
    }
    case SourceCodeStreamEncoderSettings::FloatType:
    {
        float e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        sprintf( buffer, "%f", e );
        break;
    }
    case SourceCodeStreamEncoderSettings::DoubleType:
    {
        double e = 0;
        byteArrayModel->copyTo( (char*)&e, pos, qMin(uint(sizeof(e)),dataSize) );
        sprintf( buffer, "%f", e );
        break;
    }
    default:
    {
        buffer[0] = 0;
    }
  }
}

KByteArraySourceCodeStreamEncoder::~KByteArraySourceCodeStreamEncoder() {}

#include "kbytearraysourcecodestreamencoder.moc"

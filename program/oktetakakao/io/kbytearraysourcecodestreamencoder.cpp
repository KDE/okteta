/***************************************************************************
                          kbytearraysourcecodestreamencoder.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#include "kbytearraysourcecodestreamencoder.h"

// lib
#include "kbytearraydocument.h"
#include "kbytearrayselection.h"
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
 : KAbstractByteArrayStreamEncoder( i18n("C array"), QLatin1String("text/x-csrc") )
{}


bool KByteArraySourceCodeStreamEncoder::encodeDataToStream( QIODevice *device,
                                                            const char *data, int size )
{
    bool success = true;

    QTextStream textStream( device );

    const int dataTypeSize = SizeOfPrimitiveDataType[mSettings.dataType];
    const int sizeOfArray = (size+dataTypeSize-1) / dataTypeSize;

    textStream << QLatin1String(PrimitiveDataTypeName[mSettings.dataType]) << " "
               << mSettings.variableName << "[" << sizeOfArray << "] = {" << endl;

    int elementAddedOnLine = 0;
    for( int i=0; i<size; i+=dataTypeSize )
    {
        static char buffer[12];
        printFormatted( buffer, &data[i], size-i );
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


void KByteArraySourceCodeStreamEncoder::printFormatted( char *buffer, const char *data, unsigned int dataSize ) const
{
    switch( mSettings.dataType )
    {
    case SourceCodeStreamEncoderSettings::CharType:
    {
    char e = 0;
    memcpy( &e, data, qMin<size_t>(sizeof(e),dataSize) );
    sprintf( buffer, "%d", e );
    break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedCharType:
    {
    unsigned char e = 0;
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
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
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
    sprintf( buffer, "%d", e );
    break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedShortType:
    {
    unsigned short e = 0;
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
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
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
    sprintf( buffer, "%u", e );
    break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedIntegerType:
    {
    unsigned int e = 0;
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
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
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
    sprintf( buffer, "%f", e );
    break;
    }
    case SourceCodeStreamEncoderSettings::DoubleType:
    {
    double e = 0;
    memcpy( &e, data, qMin(uint(sizeof(e)),dataSize) );
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

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearraysourcecodestreamencoder.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextStream>


namespace Kasten2
{

static const char* const PrimitiveDataTypeName[] =
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
 : variableName( QString::fromLatin1("array") ), dataType( UnsignedCharType ), elementsPerLine( 4 ), unsignedAsHexadecimal( true )
{}


ByteArraySourceCodeStreamEncoder::ByteArraySourceCodeStreamEncoder()
 : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","C Array"), QString::fromLatin1("text/x-csrc") )
{}

const char* const* ByteArraySourceCodeStreamEncoder::dataTypeNames() const { return PrimitiveDataTypeName; }
int ByteArraySourceCodeStreamEncoder::dataTypesCount() const { return NoOfPrimitiveDataTypes; }

bool ByteArraySourceCodeStreamEncoder::encodeDataToStream( QIODevice* device,
                                                            const ByteArrayView* byteArrayView,
                                                            const Okteta::AbstractByteArrayModel* byteArrayModel,
                                                            const Okteta::AddressRange& range )
{
Q_UNUSED( byteArrayView )

    bool success = true;

    // encode
    QTextStream textStream( device );

//     textStream << "// from File , selection \n";

    const int size = range.width();
    const int dataTypeSize = SizeOfPrimitiveDataType[mSettings.dataType];
    const int sizeOfArray = (size+dataTypeSize-1) / dataTypeSize;

    textStream << "const " << QLatin1String(PrimitiveDataTypeName[mSettings.dataType]) << ' '
               << mSettings.variableName << '[' << sizeOfArray << "] =" << endl
               << '{' << endl;

    int elementAddedOnLine = 0;
    for( Okteta::Address i=range.start(); i<=range.end(); i+=dataTypeSize )
    {
        if( elementAddedOnLine == 0 )
            textStream << "   "; // just 3, one space before every datum
        textStream << ' ' << printFormatted( byteArrayModel, i, range.end()-i+1 );
        if( i + dataTypeSize <= range.end() )
            textStream << ',';

        if( ++elementAddedOnLine >= mSettings.elementsPerLine )
        {
            textStream << endl;
            elementAddedOnLine = 0;
        }
    }
    if( elementAddedOnLine > 0 )
        textStream << endl;

    textStream << "};" << endl;

    return success;
}


QString ByteArraySourceCodeStreamEncoder::printFormatted( const Okteta::AbstractByteArrayModel* byteArrayModel, Okteta::Address offset,
                                                          unsigned int dataSize ) const
{
    static const char DecimalFormattedNumberPlaceHolder[] = "%1";
    static const char HexadecimalFormattedNumberPlaceHolder[] = "0x%1";

    QString result;
    switch( mSettings.dataType )
    {
    case SourceCodeStreamEncoderSettings::CharType:
    {
        char e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin<size_t>(sizeof(e),dataSize) );
        static const int fieldWidth = 4;
        result = QString::fromLatin1(DecimalFormattedNumberPlaceHolder).arg( (int)e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedCharType:
    {
        unsigned char e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 2 : 3;
        const char* FormattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            HexadecimalFormattedNumberPlaceHolder : DecimalFormattedNumberPlaceHolder;
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = QString::fromLatin1(FormattedNumberPlaceHolder).arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::ShortType:
    {
        short e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 6;
        result = QString::fromLatin1(DecimalFormattedNumberPlaceHolder).arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedShortType:
    {
        unsigned short e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 4 : 5;
        const char* FormattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            HexadecimalFormattedNumberPlaceHolder : DecimalFormattedNumberPlaceHolder;
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = QString::fromLatin1(FormattedNumberPlaceHolder).arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::IntegerType:
    {
        int e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 11;
        result = QString::fromLatin1(DecimalFormattedNumberPlaceHolder).arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::UnsignedIntegerType:
    {
        unsigned int e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 8 : 10;
        const char* FormattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            HexadecimalFormattedNumberPlaceHolder : DecimalFormattedNumberPlaceHolder;
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = QString::fromLatin1(FormattedNumberPlaceHolder).arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::FloatType:
    {
        float e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 13;
        result = QString::fromLatin1(DecimalFormattedNumberPlaceHolder).arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::DoubleType:
    {
        double e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 13;
        result = QString::fromLatin1(DecimalFormattedNumberPlaceHolder).arg( e, fieldWidth );
        break;
    }
    default:
        break;
    }

    return result;
}

ByteArraySourceCodeStreamEncoder::~ByteArraySourceCodeStreamEncoder() {}

}

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
#include <okteta/abstractbytearraymodel.h>
// KF5
#include <KLocalizedString>
// Qt
#include <QTextStream>


namespace Kasten
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

inline QString decimalFormattedNumberPlaceHolder() { return QStringLiteral("%1"); }
inline QString hexadecimalFormattedNumberPlaceHolder() { return QStringLiteral("0x%1"); }

SourceCodeStreamEncoderSettings::SourceCodeStreamEncoderSettings()
   : variableName( QStringLiteral("array") ),
     dataType( PrimitiveDataType::UnsignedChar ),
     elementsPerLine( 4 ),
     unsignedAsHexadecimal( true )
{}


ByteArraySourceCodeStreamEncoder::ByteArraySourceCodeStreamEncoder()
 : AbstractByteArrayStreamEncoder( i18nc("name of the encoding target","C Array"), QStringLiteral("text/x-csrc") )
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
    const int dataTypeSize = SizeOfPrimitiveDataType[static_cast<int>(mSettings.dataType)];
    const int sizeOfArray = (size+dataTypeSize-1) / dataTypeSize;

    textStream << "const " << QLatin1String(PrimitiveDataTypeName[static_cast<int>(mSettings.dataType)]) << ' '
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
    QString result;
    switch( mSettings.dataType )
    {
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Char:
    {
        char e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin<size_t>(sizeof(e),dataSize) );
        static const int fieldWidth = 4;
        result = decimalFormattedNumberPlaceHolder().arg( (int)e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedChar:
    {
        unsigned char e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 2 : 3;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = formattedNumberPlaceHolder.arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Short:
    {
        short e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 6;
        result = decimalFormattedNumberPlaceHolder().arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedShort:
    {
        unsigned short e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 4 : 5;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = formattedNumberPlaceHolder.arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Integer:
    {
        int e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 11;
        result = decimalFormattedNumberPlaceHolder().arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::UnsignedInteger:
    {
        unsigned int e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        const int base = mSettings.unsignedAsHexadecimal ? 16 : 10;
        const int fieldWidth = mSettings.unsignedAsHexadecimal ? 8 : 10;
        const QString formattedNumberPlaceHolder = mSettings.unsignedAsHexadecimal ?
            hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
        const QChar stuffChar = QLatin1Char( mSettings.unsignedAsHexadecimal ? '0' : ' ' );
        result = formattedNumberPlaceHolder.arg( e, fieldWidth, base, stuffChar );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Float:
    {
        float e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg( e, fieldWidth );
        break;
    }
    case SourceCodeStreamEncoderSettings::PrimitiveDataType::Double:
    {
        double e = 0;
        byteArrayModel->copyTo( reinterpret_cast<Okteta::Byte*>(&e), offset, qMin(uint(sizeof(e)),dataSize) );
        static const int fieldWidth = 13;
        result = decimalFormattedNumberPlaceHolder().arg( e, fieldWidth );
        break;
    }
    }

    return result;
}

ByteArraySourceCodeStreamEncoder::~ByteArraySourceCodeStreamEncoder() {}

}

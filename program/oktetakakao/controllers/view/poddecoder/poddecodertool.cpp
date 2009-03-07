/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "poddecodertool.h"

// lib
#include <kbytearraydocument.h>
#include <kbytearraydisplay.h>
// Okteta core
#include <khechar.h>
#include <charcodec.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QTextCodec>


static const unsigned char PrimitivesDefaultUndefinedChar = '?';

enum PODTypes
{
    BinaryId = 0,
    OctalId,
    HexadecimalId,
    Signed8BitId,
    Unsigned8BitId,
    Signed16BitId,
    Unsigned16BitId,
    Signed32BitId,
    Unsigned32BitId,
    Signed64BitId,
    Unsigned64BitId,
    Float32BitId,
    Float64BitId,
    Char8BitId,
    UTF8Id,
//     UTF16Id,
    PODTypeCount
};



PODDecoderTool::PODDecoderTool()
 : mByteArrayDisplay( 0 ), mByteArrayModel( 0 ), mCursorIndex( 0 ),
   mCharCodec( KHECore::CharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( PrimitivesDefaultUndefinedChar ),
   mUnsignedAsHex( true )
{
    setObjectName( "PODDecoder" );

    mUtf8Codec = QTextCodec::codecForName( "UTF-8" );

    setupDecoder();
}

QString PODDecoderTool::title() const { return i18nc("@title:window", "Decoding Table"); }

void PODDecoderTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;
    KByteArrayDocument *document =
        mByteArrayDisplay ? qobject_cast<KByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayModel && mByteArrayDisplay )
    {
        mCursorIndex = mByteArrayDisplay->cursorPosition();
        connect( mByteArrayDisplay, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChange( int )) );
        connect( mByteArrayModel, SIGNAL(contentsChanged( const KHE::ArrayChangeMetricsList& )),
                 SLOT(onContentsChange()) );
        onCharCodecChange( mByteArrayDisplay->charCodingName() );
        connect( mByteArrayDisplay,  SIGNAL(charCodecChanged( const QString& )),
                 SLOT(onCharCodecChange( const QString& )) );
    }

    updateData();
}


void PODDecoderTool::setupDecoder()
{
    mDecoderNameList.resize( PODTypeCount );
    mDecoderValueList.resize( PODTypeCount );

    mDecoderNameList[BinaryId] =
        i18nc("@label:textbox encoding of one byte as value in the binary format",     "Binary 8-bit:");
    mDecoderNameList[OctalId] =
        i18nc("@label:textbox encoding of one byte as value in the octal format",      "Octal 8-bit:");
    mDecoderNameList[HexadecimalId] =
        i18nc("@label:textbox encoding of one byte as value in the hexadecimal format","Hexadecimal 8-bit:");
    mDecoderNameList[Signed8BitId] =
        i18nc("@label:textbox","Signed 8-bit:");
    mDecoderNameList[Unsigned8BitId] =
        i18nc("@label:textbox","Unsigned 8-bit:");
    mDecoderNameList[Signed16BitId] =
        i18nc("@label:textbox","Signed 16-bit:");
    mDecoderNameList[Unsigned16BitId] =
        i18nc("@label:textbox","Unsigned 16-bit:");
    mDecoderNameList[Signed32BitId] =
        i18nc("@label:textbox","Signed 32-bit:");
    mDecoderNameList[Unsigned32BitId] =
        i18nc("@label:textbox","Unsigned 32-bit:");
    mDecoderNameList[Signed64BitId] =
        i18nc("@label:textbox","Signed 64-bit:");
    mDecoderNameList[Unsigned64BitId] =
        i18nc("@label:textbox","Unsigned 64-bit:");
    mDecoderNameList[Float32BitId] =
        i18nc("@label:textbox","Float 32-bit:");
    mDecoderNameList[Float64BitId] =
        i18nc("@label:textbox","Float 64-bit:");
    mDecoderNameList[Char8BitId] =
        i18nc("@label:textbox encoding of one byte as character", "Character 8-bit:");
    mDecoderNameList[UTF8Id] =
        i18nc("@label:textbox","UTF-8:");
#if 0
    mDecoderNameList[UTF16Id] =
        i18nc("@label:textbox","UTF-16:");
#endif
}

void PODDecoderTool::setUnsignedAsHex( bool unsignedAsHex )
{
    if( mUnsignedAsHex == unsignedAsHex )
        return;

    mUnsignedAsHex = unsignedAsHex;
    updateData();
}

void PODDecoderTool::setByteOrder( int byteOrder )
{
    // TODO: test on no change is done in PODData, not this level
    mPODData.setByteOrder( byteOrder );
    updateData();
}

void PODDecoderTool::onCharCodecChange( const QString& codecName )
{
    if( codecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::CharCodec::createCodec( codecName );
    updateData();
}

#if 0
void PODDecoderTool::setUndefinedChar( const QChar& undefinedChar )
{
    if( mUndefinedChar == undefinedChar )
        return;

    mUndefinedChar = undefinedChar;
    onDataChange();
}
#endif

void PODDecoderTool::onCursorPositionChange( int pos )
{
    mCursorIndex = pos;
    updateData();
}

void PODDecoderTool::onContentsChange()
{
    // TODO: only update if affected
    updateData();
}


int PODDecoderTool::podCount() const { return mDecoderValueList.count(); }


QString PODDecoderTool::nameOfPOD( int podId ) const
{
    QString result = mDecoderNameList[podId];
    return result;
}


QString PODDecoderTool::valueAsString( int podId ) const
{
    QString result = mDecoderValueList[podId];
    return result;
}

void PODDecoderTool::updateData()
{
    int dataSize;
    if( mByteArrayModel )
    {
        dataSize = mByteArrayModel->size() - mCursorIndex;
        if( dataSize > mPODData.Size )
            dataSize = mPODData.Size;
        else if( dataSize < 0 )
            dataSize = 0;
    }
    else
        dataSize = 0;

    const bool hasDataSet = ( dataSize > 0 );
    if( hasDataSet )
        mByteArrayModel->copyTo( (char*)mPODData.rawData(), mCursorIndex, mPODData.Size );

    const bool hasChanged = mPODData.updateRawData( dataSize );

    if( !hasChanged )
        return;

    static const int HexBase = 16;
    static const int OctBase = 8;
    static const int DualBase = 2;
    const QLatin1Char ZeroChar('0');
    const QString EmptyString;
    const QString hexMask = QString::fromLatin1( "0x%1" );

    const void *P8Bit, *P16Bit, *P32Bit, *P64Bit;

    mPODData.pointers( &P8Bit, &P16Bit, &P32Bit, &P64Bit );

    // unsigned 8 bit
    const QString unsigned8Bit = (P8Bit == 0) ?   EmptyString :
                                 mUnsignedAsHex ? hexMask.arg(*(unsigned char*)P8Bit,2,HexBase,ZeroChar) :
                                                  QString::number(*(unsigned char*)P8Bit);
    mDecoderValueList[Unsigned8BitId] = unsigned8Bit;

    // signed int 8 bit
    const QString signed8Bit = (P8Bit == 0) ? EmptyString :
                                              QString::number(*(signed char*)P8Bit);
    mDecoderValueList[Signed8BitId] = signed8Bit;

    // unsigned int 16 bit
    const QString unsigned16Bit = (P16Bit == 0) ?  EmptyString :
                                  mUnsignedAsHex ? hexMask.arg(*(unsigned short*)P16Bit,4,HexBase,ZeroChar) :
                                                   QString::number(*(unsigned short*)P16Bit);
    mDecoderValueList[Unsigned16BitId] = unsigned16Bit;

    // signed int 16 bit
    const QString signed16Bit = (P16Bit == 0) ? EmptyString :
                                                QString::number(*(signed short*)P16Bit);
    mDecoderValueList[Signed16BitId] = signed16Bit;

    // unsigned int 32 bit
    const QString unsigned32Bit = (P32Bit == 0) ?  EmptyString :
                                  mUnsignedAsHex ? hexMask.arg(*(unsigned int*)P32Bit,8,HexBase,ZeroChar) :
                                                   QString::number(*(unsigned int*)P32Bit);
    mDecoderValueList[Unsigned32BitId] = unsigned32Bit;

    // signed int 32 bit
    const QString signed32Bit = (P32Bit == 0) ? EmptyString :
                                                QString::number(*(signed int*)P32Bit);
    mDecoderValueList[Signed32BitId] = signed32Bit;

    // unsigned int 64 bit
    const QString unsigned64Bit = (P64Bit == 0) ?  EmptyString :
                                  mUnsignedAsHex ? hexMask.arg(*(quint64*)P64Bit,8,HexBase,ZeroChar) :
                                                   QString::number(*(quint64*)P64Bit);
    mDecoderValueList[Unsigned64BitId] = unsigned64Bit;

    // signed int 64 bit
    const QString signed64Bit = (P64Bit == 0) ? EmptyString :
                                                QString::number(*(qint64*)P64Bit);
    mDecoderValueList[Signed64BitId] = signed64Bit;

    // float 32 bit
    const QString float32Bit = (P32Bit == 0) ? EmptyString :
                                               QString::number(*(float*)P32Bit,'e');
    mDecoderValueList[Float32BitId] = float32Bit;

    // float 64 bit
    const QString double64Bit = (P64Bit == 0) ? EmptyString :
                                                QString::number(*(double*)P64Bit,'e');
    mDecoderValueList[Float64BitId] = double64Bit;

    const int numBits = 0;//mBitWidthSelection->currentIndex();
    const bool hasEnoughBits = ( (numBits>8 ? P16Bit : P8Bit) != 0 );
    if( hasEnoughBits )
    {
        const QString SimpleMask = QString::fromLatin1( "%1" );
        if( numBits <= 0 )
        {
            const unsigned char byte = *(unsigned char*)P8Bit;
            mDecoderValueList[BinaryId] = SimpleMask.arg(byte,8,DualBase,ZeroChar );
            mDecoderValueList[OctalId] = SimpleMask.arg(byte,3,OctBase,ZeroChar );
            mDecoderValueList[HexadecimalId] = SimpleMask.arg(byte,2,HexBase,ZeroChar );
        }
        else
        {
            const unsigned long bits = mPODData.bitValue( numBits );
            const QString MoreThan8BitMask = QString::fromLatin1( "%1 %2" );
            QString HexDezValue;
            QString OktalValue;
            if( numBits > 8 )
            {
                const unsigned int higherBits = (bits>>8)&0xFF;
                HexDezValue = MoreThan8BitMask.arg( higherBits, 2, HexBase, ZeroChar );
                OktalValue = MoreThan8BitMask.arg( higherBits, 3, OctBase, ZeroChar );
            }
            else
            {
                HexDezValue = SimpleMask;
                OktalValue = SimpleMask;
            }

            mDecoderValueList[BinaryId] = SimpleMask.arg(bits,numBits,DualBase,ZeroChar );

            const unsigned int lowerBits = bits&0xFF;
            mDecoderValueList[OctalId] = OktalValue.arg(lowerBits,3,OctBase,ZeroChar );
            mDecoderValueList[HexadecimalId] = HexDezValue.arg(lowerBits,2,HexBase,ZeroChar );
        }

        const KHECore::KChar decodedChar = mCharCodec->decode( *(unsigned char*)P8Bit );
        mDecoderValueList[Char8BitId] = QString(decodedChar.isUndefined()?mUndefinedChar:(QChar)decodedChar );
    }
    else
    {
        for( int i=BinaryId; i<=HexadecimalId; ++i )
            mDecoderValueList[i] = EmptyString;
        mDecoderValueList[Char8BitId] = EmptyString;
    }
    // UTF-8
    // interpreted as a sequence of bytes, there is no endian problem
    // source: http://unicode.org/faq/utf_bom.html#3
    const QChar replacementChar( QChar::ReplacementCharacter );
    const char* data = (const char*)mPODData.originalData();
    const int maxUtf8DataSize = mPODData.size();

    QString utf8;
    bool isUtf8 = false;
    for( int i=1; i<=maxUtf8DataSize; ++i )
    {
        utf8 = mUtf8Codec->toUnicode( data, i );
        if( utf8.size() == 1 && utf8[0] != replacementChar )
        {
            isUtf8 = true;
            break;
        }
    }
    mDecoderValueList[UTF8Id] = isUtf8 ? utf8 : EmptyString;

#if 0
    // UTF-16
    const int maxUtf16DataSize = mPODData.size() / 2;
    const QString utf16 = QString::fromUtf16( (ushort*)PXBit, maxUtf16DataSize ).left( 1 );
    mDecoderValueList[UTF16Id] = utf16.isEmpty() ? EmptyString : utf16;
#endif

    // TODO: only emit for those strings that changed
    emit dataChanged();
}

PODDecoderTool::~PODDecoderTool()
{
    delete mCharCodec;
}

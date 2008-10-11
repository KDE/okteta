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
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytearraymodel.h>
// KDE
#include <KLocale>

static const unsigned char PrimitivesDefaultUndefinedChar = '?';

enum PODTypes
{
    BinaryId = 0,
    OctalId = 1,
    HexadecimalId = 2,
    CharacterId = 3,
    Signed8BitId = 4,
    Unsigned8BitId = 5,
    Signed16BitId = 6,
    Unsigned16BitId = 7,
    Signed32BitId = 8,
    Unsigned32BitId = 9,
    Float32BitId = 10,
    Float64BitId = 11,
    PODTypeCount = 12
};



PODDecoderTool::PODDecoderTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 ), mCursorIndex( 0 ),
   mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( PrimitivesDefaultUndefinedChar ),
   mUnsignedAsHex( true )
{
    setObjectName( "PODDecoder" );
    setupDecoder();
    updateData();
}

QString PODDecoderTool::title() const { return i18nc("@title:window", "Decoding Table"); }

void PODDecoderTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    KAbstractView* view = model ? qobject_cast<KAbstractView*>( model ) : 0;
    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;
    KByteArrayDocument *document = view ? qobject_cast<KByteArrayDocument*>( view->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayModel && mByteArrayView )
    {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect( mByteArrayView, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChange( int )) );
        connect( mByteArrayModel, SIGNAL(contentsChanged( int, int )), SLOT(onContentsChange( int, int )) );
        onCharCodecChange( mByteArrayView->encodingName() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString& )),
                 SLOT(onCharCodecChange( const QString& )) );
    }

    updateData();
}


void PODDecoderTool::setupDecoder()
{
    mDecoderNameList.resize( PODTypeCount );
    mDecoderValueList.resize( PODTypeCount );

    mDecoderNameList[BinaryId] =
        i18nc("@label:textbox encoding of the bytes as value in the binary format",     "Binary:");
    mDecoderNameList[OctalId] =
        i18nc("@label:textbox encoding of the bytes as value in the octal format",      "Octal:");
    mDecoderNameList[HexadecimalId] =
        i18nc("@label:textbox encoding of the bytes as value in the hexadecimal format","Hexadecimal:");
    mDecoderNameList[CharacterId] =
        i18nc("@label:textbox encoding of the bytes as character",                      "Character:");
    mDecoderNameList[Signed8BitId] =
        i18nc("@label:textbox","Signed 8 bit:");
    mDecoderNameList[Unsigned8BitId] =
        i18nc("@label:textbox","Unsigned 8 bit:");
    mDecoderNameList[Signed16BitId] =
        i18nc("@label:textbox","Signed 16 bit:");
    mDecoderNameList[Unsigned16BitId] =
        i18nc("@label:textbox","Unsigned 16 bit:");
    mDecoderNameList[Signed32BitId] =
        i18nc("@label:textbox","Signed 32 bit:");
    mDecoderNameList[Unsigned32BitId] =
        i18nc("@label:textbox","Unsigned 32 bit:");
    mDecoderNameList[Float32BitId] =
        i18nc("@label:textbox","32 bit float:");
    mDecoderNameList[Float64BitId] =
        i18nc("@label:textbox","64 bit float:");
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
    mCharCodec = KHECore::KCharCodec::createCodec( codecName );
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

void PODDecoderTool::onContentsChange( int start, int end )
{
    if( start-mPODData.Size < mCursorIndex && mCursorIndex <= end )
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
    const QString EmptyNote( '-' );
    const QString hexMask = QString::fromLatin1( "0x%1" );

    const void *P8Bit, *P16Bit, *P32Bit, *P64Bit;

    mPODData.pointers( &P8Bit, &P16Bit, &P32Bit, &P64Bit );

    // unsigned 8 bit
    const QString unsigned8Bit = (P8Bit == 0) ?   EmptyNote :
                                 mUnsignedAsHex ? hexMask.arg(*(unsigned char*)P8Bit,2,HexBase,ZeroChar) :
                                                  QString::number(*(unsigned char*)P8Bit);
    mDecoderValueList[Unsigned8BitId] = unsigned8Bit;

    // signed int 8 bit
    const QString signed8Bit = (P8Bit == 0) ? EmptyNote :
                                              QString::number(*(signed char*)P8Bit);
    mDecoderValueList[Signed8BitId] = signed8Bit;

    // unsigned int 16 bit
    const QString unsigned16Bit = (P16Bit == 0) ?  EmptyNote :
                                  mUnsignedAsHex ? hexMask.arg(*(unsigned short*)P16Bit,4,HexBase,ZeroChar) :
                                                   QString::number(*(unsigned short*)P16Bit);
    mDecoderValueList[Unsigned16BitId] = unsigned16Bit;

    // signed int 16 bit
    const QString signed16Bit = (P16Bit == 0) ? EmptyNote :
                                                QString::number(*(signed short*)P16Bit);
    mDecoderValueList[Signed16BitId] = signed16Bit;

    // unsigned int 32 bit
    const QString unsigned32Bit = (P32Bit == 0) ?  EmptyNote :
                                  mUnsignedAsHex ? hexMask.arg(*(unsigned int*)P32Bit,8,HexBase,ZeroChar) :
                                                   QString::number(*(unsigned int*)P32Bit);
    mDecoderValueList[Unsigned32BitId] = unsigned32Bit;

    // signed int 32 bit
    const QString signed32Bit = (P32Bit == 0) ? EmptyNote :
                                                QString::number(*(signed int*)P32Bit);
    mDecoderValueList[Signed32BitId] = signed32Bit;

    // float 32 bit
    const QString float32Bit = (P32Bit == 0) ? EmptyNote :
                                               QString::number(*(float*)P32Bit,'e');
    mDecoderValueList[Float32BitId] = float32Bit;

    // float 64 bit
    const QString double64Bit = (P64Bit == 0) ? EmptyNote :
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
        mDecoderValueList[CharacterId] = QString(decodedChar.isUndefined()?mUndefinedChar:(QChar)decodedChar );
    }
    else
        for( int i=BinaryId; i<=CharacterId; ++i )
            mDecoderValueList[i] = EmptyNote;

    // TODO: only emit for those strings that changed
    emit dataChanged();
}

PODDecoderTool::~PODDecoderTool()
{
    delete mCharCodec;
}


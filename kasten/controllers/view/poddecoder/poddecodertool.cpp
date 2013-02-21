/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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
#include "typecodecs/binary8codec.h"
#include "typecodecs/octal8codec.h"
#include "typecodecs/hexadecimal8codec.h"
#include "typecodecs/uint8codec.h"
#include "typecodecs/uint16codec.h"
#include "typecodecs/uint32codec.h"
#include "typecodecs/uint64codec.h"
#include "typecodecs/sint8codec.h"
#include "typecodecs/sint16codec.h"
#include "typecodecs/sint32codec.h"
#include "typecodecs/sint64codec.h"
#include "typecodecs/float32codec.h"
#include "typecodecs/float64codec.h"
#include "typecodecs/char8codec.h"
#include "typecodecs/utf8codec.h"
#include "abstracttypecodec.h"
#include "abstractdifferentsizedialog.h"
#include <bytearraydocument.h>
#include <bytearrayview.h>
// Okteta core
#include <charcodec.h>
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>


namespace Kasten2
{

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
  : mByteArrayView( 0 ),
    mByteArrayModel( 0 ),
    mCursorIndex( 0 ),
    mReadOnly( true ),
    mIsPodMarked( false ),
    mCharCodec( Okteta::CharCodec::createCodec(Okteta::LocalEncoding) ),
    mDifferentSizeDialog( 0 ),
    mUnsignedAsHex( true )
{
    setObjectName( QLatin1String( "PODDecoder" ) );

    setupDecoder();
}

QString PODDecoderTool::title() const { return i18nc("@title:window", "Decoding Table"); }
bool PODDecoderTool::isReadOnly() const { return mReadOnly; }
bool PODDecoderTool::isApplyable() const { return (mByteArrayModel != 0); }

void PODDecoderTool::setTargetModel( AbstractModel* model )
{
    const bool oldIsApplyable = isApplyable();

    if( mByteArrayView )
    {
        mByteArrayView->disconnect( this );
        if( mIsPodMarked )
            unmarkPOD();
    }
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;
    ByteArrayDocument *document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayModel && mByteArrayView )
    {
        mCursorIndex = mByteArrayView->cursorPosition();
        connect( mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)),
                 SLOT(onCursorPositionChange(Okteta::Address)) );
        connect( mByteArrayModel, SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                 SLOT(onContentsChange()) );
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 SLOT(onCharCodecChange(QString)) );
        connect( mByteArrayView, SIGNAL(readOnlyChanged(bool)),
                 SLOT(onReadOnlyChanged()) );
        onCharCodecChange( mByteArrayView->charCodingName() );
    }

    updateData();
    onReadOnlyChanged();
    const bool newIsApplyable = isApplyable();
    if( oldIsApplyable != newIsApplyable )
        emit isApplyableChanged( newIsApplyable );
}


void PODDecoderTool::setupDecoder()
{
    mTypeCodecs.resize( PODTypeCount );
    mTypeCodecs[BinaryId] =        new Okteta::Binary8Codec();
    mTypeCodecs[OctalId] =         new Okteta::Octal8Codec();
    mTypeCodecs[HexadecimalId] =   new Okteta::Hexadecimal8Codec();
    mTypeCodecs[Signed8BitId] =    new Okteta::SInt8Codec();
    mTypeCodecs[Unsigned8BitId] =  new Okteta::UInt8Codec();
    mTypeCodecs[Signed16BitId] =   new Okteta::SInt16Codec();
    mTypeCodecs[Unsigned16BitId] = new Okteta::UInt16Codec();
    mTypeCodecs[Signed32BitId] =   new Okteta::SInt32Codec();
    mTypeCodecs[Unsigned32BitId] = new Okteta::UInt32Codec();
    mTypeCodecs[Signed64BitId] =   new Okteta::SInt64Codec();
    mTypeCodecs[Unsigned64BitId] = new Okteta::UInt64Codec();
    mTypeCodecs[Float32BitId] =    new Okteta::Float32Codec();
    mTypeCodecs[Float64BitId] =    new Okteta::Float64Codec();
    mTypeCodecs[Char8BitId] =      new Okteta::Char8Codec( mCharCodec );
    mTypeCodecs[UTF8Id] =          new Okteta::Utf8Codec();

#if 0
    mDecoderNameList[UTF16Id] =
        i18nc("@label:textbox","UTF-16:");
#endif

    mDecodedValueList.resize( PODTypeCount );
    mDecodedValueByteCountList.resize( PODTypeCount );
}

void PODDecoderTool::setDifferentSizeDialog( AbstractDifferentSizeDialog* differentSizeDialog )
{
    mDifferentSizeDialog = differentSizeDialog;
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
    mPODData.setByteOrder( (Okteta::ByteOrder)byteOrder );
    updateData();
}

void PODDecoderTool::onCharCodecChange( const QString& codecName )
{
    if( codecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec( codecName );
    static_cast<Okteta::Char8Codec*>( mTypeCodecs[Char8BitId] )->setCharCodec( mCharCodec );
    updateData();
}


void PODDecoderTool::onCursorPositionChange( Okteta::Address pos )
{
    mCursorIndex = pos;
    updateData();
}

void PODDecoderTool::onContentsChange()
{
    // TODO: only update if affected
    updateData();
}


int PODDecoderTool::podCount() const { return mTypeCodecs.count(); }


QString PODDecoderTool::nameOfPOD( int podId ) const
{
    return mTypeCodecs[podId]->name();
}


QVariant PODDecoderTool::value( int podId ) const
{
    // TODO: add caching here
    return mDecodedValueList[podId];
}


void PODDecoderTool::setData( const QVariant& data, int podId )
{
    Okteta::AbstractTypeCodec* typeCodec = mTypeCodecs[podId];

    // QVariant::operator=() only compares values' addresses for custom types,
    // so the comparison for values needs to be done by someone with knowledge about the type.
    const bool isUnchangedValue = typeCodec->areEqual( data, mDecodedValueList[podId] );

    if( isUnchangedValue )
        return;

    QByteArray bytes = typeCodec->valueToBytes( data );

    const int bytesSize = bytes.size();
    if( bytesSize == 0 )
        return;

    // need to swap the bytes
    if( mPODData.byteOrder() != Okteta::thisMachineByteOrder )
    {
        const int firstHalfBytesCount = bytesSize/2;
        int j = bytesSize - 1;
        for( int i=0; i<firstHalfBytesCount; ++i,--j )
        {
            const char helper = bytes[i];
            bytes[i] = bytes[j];
            bytes[j] = helper;
        }
    }

    const int oldValueSize = mDecodedValueByteCountList[podId];
    int removedBytesSize = bytesSize;
    if( bytesSize != oldValueSize )
    {
//         const int sizeLeft = mByteArrayModel->size() - mCursorIndex;
        const Answer answer = Cancel; // TODO: non-persistent editor closes on new dialog -> crash after dialog
//             mDifferentSizeDialog ? mDifferentSizeDialog->query( bytesSize, oldValueSize, sizeLeft ) : Cancel;
        if( answer == Cancel )
            return;

        if( answer == AdaptSize )
            removedBytesSize = oldValueSize;
    }

    Okteta::ChangesDescribable* changesDescribable =
        qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

    if( changesDescribable )
        changesDescribable->openGroupedChange( i18nc("Edited as %datatype","Edited as %1", typeCodec->name()) );
    mByteArrayModel->replace( Okteta::AddressRange::fromWidth(mCursorIndex,removedBytesSize), bytes );
    if( changesDescribable )
        changesDescribable->closeGroupedChange();
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
        mByteArrayModel->copyTo( mPODData.rawData(), mCursorIndex, mPODData.Size );

    const bool hasChanged = mPODData.updateRawData( dataSize );

    if( ! hasChanged )
        return;

    // TODO: only calculate on demand + cache
    for( int podId=0; podId<PODTypeCount; ++podId )
    {
        int byteCount = 0;
        mDecodedValueList[podId] = mTypeCodecs[podId]->value( mPODData, &byteCount );
        mDecodedValueByteCountList[podId] = byteCount;
    }

    // TODO: only emit for those strings that changed
    emit dataChanged();
}


void PODDecoderTool::markPOD( int podId )
{
    const int length = mDecodedValueByteCountList[podId];
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth( mCursorIndex, length );
    mByteArrayView->setMarking( markingRange, true );
    mIsPodMarked = true;
}

void PODDecoderTool::unmarkPOD()
{
// TODO: marked region is property of document, not view?
    mByteArrayView->setMarking( Okteta::AddressRange() );
    mIsPodMarked = false;
}

void PODDecoderTool::onReadOnlyChanged()
{
    const bool newReadOnly = ( (! mByteArrayModel) || (! mByteArrayView)
                               || mByteArrayView->isReadOnly() );
    if( newReadOnly != mReadOnly )
    {
        mReadOnly = newReadOnly;
        emit readOnlyChanged( newReadOnly );
    }
}

PODDecoderTool::~PODDecoderTool()
{
    delete mCharCodec;
    qDeleteAll( mTypeCodecs );
}

}

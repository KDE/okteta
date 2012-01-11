/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "charsetconversiontool.h"

// tool
#include "charsetconversionjob.h"
// Okteta Kasten gui
#include <bytearrayview.h>
// Okteta Kasten core
#include <bytearraydocument.h>
// Okteta core
#include <charcodec.h>
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

CharsetConversionTool::CharsetConversionTool()
  : mConversionDirection( ConvertFrom ),
    mSubstitutingMissingChars(false),
    mSubstituteByte( 0 ),
    mByteArrayView( 0 ),
    mByteArrayModel( 0 )
{
    setObjectName( QLatin1String("CharsetConversion") );
}

bool CharsetConversionTool::isApplyable() const
{
    return ( mByteArrayModel &&
             mByteArrayView && mByteArrayView->hasSelectedData() &&
             ! mOtherCharCodecName.isEmpty() &&
             mByteArrayView->charCodingName() != mOtherCharCodecName );
}

QString CharsetConversionTool::title() const
{
    return i18nc("@title:window of the tool to convert between charsets",
                 "Charset Conversion");
}

QString CharsetConversionTool::otherCharCodecName() const
{
    return mOtherCharCodecName;
}

CharsetConversionTool::ConversionDirection CharsetConversionTool::conversionDirection() const
{
    return mConversionDirection;
}

bool CharsetConversionTool::isSubstitutingMissingChars() const
{
    return mSubstitutingMissingChars;
}

Okteta::Byte CharsetConversionTool::substituteByte() const
{
    return mSubstituteByte;
}


void CharsetConversionTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 SLOT(onViewChanged()) );
        connect( mByteArrayView,  SIGNAL(selectedDataChanged(const Kasten2::AbstractModelSelection*)),
                 SLOT(onViewChanged()) );
    }

    onViewChanged();
}

void CharsetConversionTool::setOtherCharCodecName( const QString& codecName )
{
    if( codecName == mOtherCharCodecName )
        return;

    mOtherCharCodecName = codecName;
    emit isApplyableChanged( isApplyable() );
}

void CharsetConversionTool::setConversionDirection( int conversionDirection )
{
    mConversionDirection = (ConversionDirection)conversionDirection;
}

void CharsetConversionTool::setSubstitutingMissingChars( bool isSubstitutingMissingChars )
{
    mSubstitutingMissingChars = isSubstitutingMissingChars;
}

void CharsetConversionTool::setSubstituteByte( int byte )
{
    mSubstituteByte = (Okteta::Byte)byte;
}


void CharsetConversionTool::convertChars()
{
    QApplication::setOverrideCursor( Qt::WaitCursor );

    const Okteta::AddressRange convertedSection = mByteArrayView->selection();
    QByteArray conversionResult;
    conversionResult.resize( convertedSection.width() );

    Okteta::CharCodec* viewCharCodec =
        Okteta::CharCodec::createCodec( mByteArrayView->charCodingName() );
    Okteta::CharCodec* otherCharCodec =
        Okteta::CharCodec::createCodec( mOtherCharCodecName );
    const bool convertToOther = (mConversionDirection == ConvertTo);
    Okteta::CharCodec* fromCharCodec = convertToOther ? viewCharCodec : otherCharCodec;
    Okteta::CharCodec* toCharCodec = convertToOther ? otherCharCodec : viewCharCodec;
    CharsetConversionJob* charsetConversionJob =
        new CharsetConversionJob( reinterpret_cast<Okteta::Byte*>(conversionResult.data()),
                                  mByteArrayModel, convertedSection,
                                  convertToOther ? viewCharCodec : otherCharCodec,
                                  convertToOther ? otherCharCodec : viewCharCodec,
                                  mSubstitutingMissingChars, mSubstituteByte
                                ); // TODO: report also actually converted bytes
    const bool success = charsetConversionJob->exec();

    if( success ) //TODO: if nothing needed to be converted, just report and don't add change
    {
        Okteta::ChangesDescribable *changesDescribable =
            qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

        if( changesDescribable )
        {
            const QString description =
                i18nc("Converted from charset 1 to charset 2",
                      "%1 to %2", fromCharCodec->name(), toCharCodec->name());
            changesDescribable->openGroupedChange( description );
        }
        mByteArrayModel->replace( convertedSection, conversionResult );
        if( changesDescribable )
            changesDescribable->closeGroupedChange();
    }

    delete viewCharCodec;
    delete otherCharCodec;

    QApplication::restoreOverrideCursor();

    const QMap<Okteta::Byte, int>& failedPerByteCount = charsetConversionJob->failedPerByteCount();
    const int convertedBytesCount = charsetConversionJob->convertedBytesCount();

    mByteArrayView->setFocus();

    emit conversionDone( success, convertedBytesCount, failedPerByteCount );
}


void CharsetConversionTool::onViewChanged()
{
    emit isApplyableChanged( isApplyable() );
}

CharsetConversionTool::~CharsetConversionTool()
{
}

}

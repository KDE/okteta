/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "stringsextracttool.h"

// lib
#include "extractstringsjob.h"
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <charcodec.h>
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

static const int DefaultMinLength = 3;

StringsExtractTool::StringsExtractTool()
  : mExtractedStringsUptodate( false ),
    mSourceByteArrayModelUptodate( false ),
    mCharCodec( Okteta::CharCodec::createCodec(Okteta::LocalEncoding) ),
    mMinLength( DefaultMinLength ),
    mByteArrayView( 0 ),
    mByteArrayModel( 0 ),
    mSourceByteArrayView( 0 ),
    mSourceByteArrayModel( 0 ),
    mSourceMinLength( 0 )
{
    setObjectName( QLatin1String( "Strings" ) );
}

bool StringsExtractTool::isApplyable() const
{
    return ( mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData() && mMinLength > 0 );
}

bool StringsExtractTool::canHighlightString() const
{
    return ( mSourceByteArrayModel == mByteArrayModel
             && mByteArrayView && mSourceByteArrayModelUptodate );
}

QString StringsExtractTool::title() const { return i18nc("@title:window of the tool to extract strings", "Strings"); }

// TODO: add model with offset and string
// doubleclick moves cursor to offset
// filter für Suche, inkl. Regulärausdrücke
// groß/kleinschreibung
// voll strings, auch mit Leerzeichen
void StringsExtractTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 SLOT(setCharCodec(QString)) );
        connect( mByteArrayView,  SIGNAL(selectedDataChanged(const Kasten2::AbstractModelSelection*)),
                 SLOT(onSelectionChanged()) );

        setCharCodec( mByteArrayView->charCodingName() );
    }

    // TODO: if there is no view, there is nothing to extract.
    // or this could be the view where we got the strings from and it did not change
    checkUptoDate();
    emit uptodateChanged( mExtractedStringsUptodate );
    emit isApplyableChanged( isApplyable() );
    emit canHighlightStringChanged( canHighlightString() );
}

void StringsExtractTool::setMinLength( int minLength )
{
    mMinLength = minLength;
    checkUptoDate();
    emit uptodateChanged( mExtractedStringsUptodate );
}

void StringsExtractTool::setCharCodec( const QString &codecName )
{
    if( codecName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec( codecName );
}

void StringsExtractTool::checkUptoDate()
{
    mExtractedStringsUptodate =
        ( mSourceByteArrayModel == mByteArrayModel
          && mByteArrayView && mSourceSelection == mByteArrayView->selection()
          && mSourceMinLength == mMinLength
          && mSourceByteArrayModelUptodate );
}

void StringsExtractTool::markString( int stringId )
{
    if( mSourceByteArrayView != mByteArrayView )
    {
        if( mSourceByteArrayView ) mSourceByteArrayView->disconnect( this );
        mSourceByteArrayView = mByteArrayView;
        connect( mSourceByteArrayView,  SIGNAL(destroyed()),
                 SLOT(onSourceViewDestroyed()) );
    }
    const ContainedString &containedString = mContainedStringList.at( stringId );
    const Okteta::Address offset = containedString.offset();
    const int length = containedString.string().length();
    const Okteta::AddressRange markingRange = Okteta::AddressRange::fromWidth( offset, length );
    mSourceByteArrayView->setMarking( markingRange, true );
}

void StringsExtractTool::unmarkString()
{
// TODO: marked region is property of document, not view?
    if( mSourceByteArrayView )
        mSourceByteArrayView->setMarking( Okteta::AddressRange() );
}

void StringsExtractTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    checkUptoDate();
    emit uptodateChanged( mExtractedStringsUptodate );
    emit isApplyableChanged( isApplyable() );
}

void StringsExtractTool::onSourceChanged()
{
    mExtractedStringsUptodate = false;
    mSourceByteArrayModelUptodate = false;
    emit uptodateChanged( false );
    emit canHighlightStringChanged( false );
}

void StringsExtractTool::onSourceDestroyed()
{
    mSourceByteArrayModel = 0;
    onSourceChanged();
}

void StringsExtractTool::onSourceViewDestroyed()
{
    mSourceByteArrayView = 0;
}


// TODO: use WordByteArrayService
void StringsExtractTool::extractStrings()
{
    // forget old string source
    if( mSourceByteArrayModel ) mSourceByteArrayModel->disconnect( this );

    QApplication::setOverrideCursor( Qt::WaitCursor );

    ExtractStringsJob *extractStringsJob =
        new ExtractStringsJob( mByteArrayModel, mByteArrayView->selection(), mCharCodec, mMinLength,
                               &mContainedStringList );
    extractStringsJob->exec();

    QApplication::restoreOverrideCursor();

    // remember new string source
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = mByteArrayView->selection();
    mSourceMinLength = mMinLength;
    connect( mSourceByteArrayModel,  SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
             SLOT(onSourceChanged()) );
    connect( mSourceByteArrayModel,  SIGNAL(destroyed()),
             SLOT(onSourceDestroyed()) );

    mExtractedStringsUptodate = true;
    mSourceByteArrayModelUptodate = true;
    emit uptodateChanged( true );
    emit canHighlightStringChanged( true );
}


StringsExtractTool::~StringsExtractTool()
{
    delete mCharCodec;
}

}

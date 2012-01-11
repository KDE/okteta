/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "viewstatuscontroller.h"

// lib
#include <bytearrayview.h>
// Kasten ui
#include <togglebutton.h>
#include <statusbar.h>
// Okteta core
#include <charcodec.h>
#include <oktetacore.h>
// KDE
#include <KSqueezedTextLabel>
#include <KComboBox>
#include <KLocale>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QFontMetrics>

#include <KDebug>

// TODO: make status bar capable to hide entries if size is too small, use priorisation

namespace Kasten2
{

ViewStatusController::ViewStatusController( StatusBar* statusBar )
 : mByteArrayView( 0 ), mStatusBar( statusBar )
{
    mPrintFunction = Okteta::OffsetFormat::printFunction( Okteta::OffsetFormat::Hexadecimal );

    mOffsetLabel = new QLabel( statusBar );
    statusBar->addWidget( mOffsetLabel );

    mSelectionLabel = new QLabel( statusBar );
    statusBar->addWidget( mSelectionLabel );

    const QString insertModeText = i18nc( "@info:status short for: Insert mode",    "INS" );
    const QString overwriteModeText = i18nc( "@info:status short for: Overwrite mode", "OVR" );
    const QString insertModeTooltip = i18nc( "@info:tooltip", "Insert mode" );
    const QString overwriteModeTooltip = i18nc( "@info:tooltip", "Overwrite mode" );
    mOverwriteModeToggleButton = new ToggleButton( insertModeText, insertModeTooltip, statusBar );
    mOverwriteModeToggleButton->setCheckedState( overwriteModeText, overwriteModeTooltip );
    statusBar->addWidget( mOverwriteModeToggleButton );
    connect( mOverwriteModeToggleButton, SIGNAL(clicked(bool)), SLOT(setOverwriteMode(bool)) );

// #if 0
    mValueCodingComboBox = new KComboBox( statusBar );
    QStringList list;
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format","Hexadecimal") );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "Decimal")     );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "Octal")       );
    list.append( i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "Binary")      );
    mValueCodingComboBox->addItems( list );
    mValueCodingComboBox->setToolTip(
        i18nc("@info:tooltip","Coding of the value interpretation in the current view.") );
    connect( mValueCodingComboBox, SIGNAL(activated(int)), SLOT(setValueCoding(int)) );
    statusBar->addWidget( mValueCodingComboBox );

    mCharCodingComboBox = new KComboBox( statusBar );
    mCharCodingComboBox->addItems( Okteta::CharCodec::codecNames() );
    mCharCodingComboBox->setToolTip(
        i18nc("@info:tooltip","Encoding in the character column of the current view.") );
    connect( mCharCodingComboBox, SIGNAL(activated(int)), SLOT(setCharCoding(int)) );
    statusBar->addWidget( mCharCodingComboBox );
// #endif

    fixWidths();

    setTargetModel( 0 );
}


// the letter C can be very wide, that is why with proportianl fonts there seems too much space used, but isn't
// see http://frinring.wordpress.com/2008/10/14/better-width-with-open-sources/
void ViewStatusController::fixWidths()
{
    const QFontMetrics metrics = mStatusBar->fontMetrics();

    // mOffsetLabel
    static const char HexDigitsCount = 16;
    static const char FirstLetterIndex = 10;
    static const char HexDigits[HexDigitsCount] =
    {
        '0','1','2','3','4','5','6','7','8','9',
        'A','B','C','D','E','F'
    };

    int largestOffsetWidth = 0;
    int largestSelectionWidth = 0;
    int widestDigitIndex = 0;
    for( int i=0; i<HexDigitsCount; ++i )
    {
        QString offset = QString( 9, QLatin1Char(HexDigits[i]) );
        offset[4] = QLatin1Char(':');
        const QString offsetText = i18n( "Offset: %1", offset );
        const int offsetWidth = metrics.boundingRect( offsetText ).width();
        if( largestOffsetWidth < offsetWidth )
            largestOffsetWidth = offsetWidth;

        const char countDigit = (i<FirstLetterIndex) ? HexDigits[i] : widestDigitIndex;
        const int maxNumber = QByteArray('1'+QByteArray(9,countDigit)).toInt();
        const QString bytesCount = i18n( "%1 bytes", maxNumber );
        const QString selectionString = i18nc( "@info:status selection: start offset - end offset ()",
                        "Selection: %1 - %2 (%3)", offset, offset, bytesCount );

        const int selectionWidth = metrics.boundingRect( selectionString ).width();
        if( largestSelectionWidth < selectionWidth )
        {
            if( i < FirstLetterIndex )
                widestDigitIndex = 1;
            largestSelectionWidth = selectionWidth;
        }
    }
    mOffsetLabel->setFixedWidth( largestOffsetWidth );
    mSelectionLabel->setFixedWidth( largestSelectionWidth );
// kDebug()<<"offset"<<largestOffsetWidth<<"selection"<<largestSelectionWidth;
}

void ViewStatusController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView )
    {
        mByteArrayView->disconnect( this );
        mByteArrayView->disconnect( mOverwriteModeToggleButton );
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    if( hasView )
    {
        mStartOffset = mByteArrayView->startOffset();

        onCursorPositionChanged( mByteArrayView->cursorPosition() );
        onSelectedDataChanged( mByteArrayView->modelSelection() );
        mOverwriteModeToggleButton->setChecked( mByteArrayView->isOverwriteMode() );
        onValueCodingChanged( (int)mByteArrayView->valueCoding() );
        onCharCodecChanged( mByteArrayView->charCodingName() );

        connect( mByteArrayView, SIGNAL(cursorPositionChanged(Okteta::Address)), SLOT(onCursorPositionChanged(Okteta::Address)) );
        connect( mByteArrayView, SIGNAL(selectedDataChanged(const Kasten2::AbstractModelSelection*)),
            SLOT(onSelectedDataChanged(const Kasten2::AbstractModelSelection*)) );
        connect( mByteArrayView, SIGNAL(overwriteModeChanged(bool)),
                 mOverwriteModeToggleButton, SLOT(setChecked(bool)) );
        connect( mByteArrayView, SIGNAL(valueCodingChanged(int)), SLOT(onValueCodingChanged(int)) );
        connect( mByteArrayView, SIGNAL(charCodecChanged(QString)),
            SLOT(onCharCodecChanged(QString)) );
    }
    else
    {
        mOffsetLabel->setText( i18nc("@info:status offset value not available", "Offset: -") );
        mSelectionLabel->setText( i18nc("@info:status offset value not available", "Selection: -") );
        mOverwriteModeToggleButton->setChecked( false );
        mValueCodingComboBox->setCurrentIndex( 0 );
        mCharCodingComboBox->setCurrentIndex( 0 );
    }

    mOffsetLabel->setEnabled( hasView );
    mSelectionLabel->setEnabled( hasView );
    mOverwriteModeToggleButton->setEnabled( hasView );
    mValueCodingComboBox->setEnabled( hasView );
    mCharCodingComboBox->setEnabled( hasView );
}

void ViewStatusController::setOverwriteMode( bool overwrite )
{
    mByteArrayView->setOverwriteMode( overwrite );
}

// #if 0
void ViewStatusController::setValueCoding( int valueCoding )
{
    mByteArrayView->setValueCoding( valueCoding );
    mByteArrayView->setFocus();
}

void ViewStatusController::setCharCoding( int charCoding )
{
    mByteArrayView->setCharCoding( Okteta::CharCodec::codecNames()[charCoding] );
    mByteArrayView->setFocus();
}
// #endif

void ViewStatusController::onCursorPositionChanged( Okteta::Address offset )
{
    static char codedOffset[Okteta::OffsetFormat::MaxFormatWidth+1];

    mPrintFunction( codedOffset, mStartOffset + offset );

    mOffsetLabel->setText( i18n("Offset: %1", QLatin1String(codedOffset)) );
}

// TODO: fix selection by cursor not sending updates
void ViewStatusController::onSelectedDataChanged( const Kasten2::AbstractModelSelection* modelSelection )
{
    const ByteArraySelection* byteArraySelection = static_cast<const ByteArraySelection*>( modelSelection );
    const Okteta::AddressRange selection = byteArraySelection->range();

    QString selectionString;
    if( ! selection.isEmpty() )
    {
        static char codedSelectionStart[Okteta::OffsetFormat::MaxFormatWidth+1];
        static char codedSelectionEnd[Okteta::OffsetFormat::MaxFormatWidth+1];

        mPrintFunction( codedSelectionStart, mStartOffset + selection.start() );
        mPrintFunction( codedSelectionEnd,   mStartOffset + selection.end() );

        const QString bytesCount = i18np( "1 byte", "%1 bytes", selection.width() );
        selectionString = i18nc( "@info:status selection: start offset - end offset (number of bytes)",
                                 "Selection: %1 - %2 (%3)", QLatin1String(codedSelectionStart), QLatin1String(codedSelectionEnd), bytesCount );
    }
    else
        selectionString = i18nc( "@info:status offset value not available", "Selection: -" );

    mSelectionLabel->setText( selectionString );
}
// #if 0
void ViewStatusController::onValueCodingChanged( int valueCoding )
{
    mValueCodingComboBox->setCurrentIndex( valueCoding );
}

void ViewStatusController::onCharCodecChanged( const QString& charCodecName )
{
    const int charCodingIndex = Okteta::CharCodec::codecNames().indexOf( charCodecName );

    mCharCodingComboBox->setCurrentIndex( charCodingIndex );
}
// #endif

}

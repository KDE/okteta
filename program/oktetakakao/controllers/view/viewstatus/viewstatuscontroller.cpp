/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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
#include <kbytearraydisplay.h>
// Kakao ui
#include <togglebutton.h>
#include <statusbar.h>
// Okteta core
#include <charcodec.h>
#include <khe.h>
// KDE
#include <KSqueezedTextLabel>
#include <KComboBox>
#include <KLocale>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QFontMetrics>

#include <KDebug>

// TODO: make status bar capable to hide entries if size is too small, use priorisation

ViewStatusController::ViewStatusController( Statusbar* statusBar )
 : mByteArrayDisplay( 0 ), mStatusBar( statusBar )
{
    mPrintFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

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
    mCharCodingComboBox->addItems( KHECore::CharCodec::codecNames() );
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
        QString offset = QString( 9, HexDigits[i] );
        offset[4] = ':';
        const QString offsetText = i18n( "Offset: %1", offset );
        const int offsetWidth = metrics.boundingRect( offsetText ).width();
        if( largestOffsetWidth < offsetWidth )
            largestOffsetWidth = offsetWidth;

        const char countDigit = (i<FirstLetterIndex) ? HexDigits[i] : widestDigitIndex;
        const QString bytesCount = i18np( "1 byte", "%1 bytes", ('1'+QString(9,countDigit)).toInt() );
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
    if( mByteArrayDisplay )
    {
        mByteArrayDisplay->disconnect( this );
        mByteArrayDisplay->disconnect( mOverwriteModeToggleButton );
    }

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    const bool hasView = ( mByteArrayDisplay != 0 );
    if( hasView )
    {
        mStartOffset = mByteArrayDisplay->startOffset();

        onCursorPositionChanged( mByteArrayDisplay->cursorPosition() );
        onHasSelectedDataChanged( mByteArrayDisplay->hasSelectedData() );
        mOverwriteModeToggleButton->setChecked( mByteArrayDisplay->isOverwriteMode() );
        onValueCodingChanged( (int)mByteArrayDisplay->valueCoding() );
        onCharCodecChanged( mByteArrayDisplay->charCodingName() );

        connect( mByteArrayDisplay, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChanged( int )) );
        connect( mByteArrayDisplay, SIGNAL(hasSelectedDataChanged( bool )), SLOT(onHasSelectedDataChanged( bool )) );
        connect( mByteArrayDisplay, SIGNAL(overwriteModeChanged( bool )),
                 mOverwriteModeToggleButton, SLOT(setChecked( bool )) );
        connect( mByteArrayDisplay, SIGNAL(valueCodingChanged( int )), SLOT(onValueCodingChanged( int )) );
        connect( mByteArrayDisplay, SIGNAL(charCodecChanged( const QString& )),
            SLOT(onCharCodecChanged( const QString& )) );
    }
    else
    {
        const QString emptyString( '-' );

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
    mByteArrayDisplay->setOverwriteMode( overwrite );
}

// #if 0
void ViewStatusController::setValueCoding( int valueCoding )
{
    mByteArrayDisplay->setValueCoding( valueCoding );
    mByteArrayDisplay->setFocus();
}

void ViewStatusController::setCharCoding( int charCoding )
{
    mByteArrayDisplay->setCharCoding( KHECore::CharCodec::codecNames()[charCoding] );
    mByteArrayDisplay->setFocus();
}
// #endif

void ViewStatusController::onCursorPositionChanged( int offset )
{
    static char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];

    mPrintFunction( codedOffset, mStartOffset + offset );

    mOffsetLabel->setText( i18n("Offset: %1", QLatin1String(codedOffset)) );
}

// TODO: we need signal selectionChanged( selection ), also fix selection by cursor not sending updates
void ViewStatusController::onHasSelectedDataChanged( bool hasSelectedData )
{
    QString string;

    if( hasSelectedData )
    {
        static char codedSelectionStart[KHEUI::KOffsetFormat::MaxFormatWidth+1];
        static char codedSelectionEnd[KHEUI::KOffsetFormat::MaxFormatWidth+1];

        const KHE::Section selection = mByteArrayDisplay->selection();
        mPrintFunction( codedSelectionStart, mStartOffset + selection.start() );
        mPrintFunction( codedSelectionEnd,   mStartOffset + selection.end() );

        const QString bytesCount = i18np( "1 byte", "%1 bytes", selection.width() );
        string = i18nc( "@info:status selection: start offset - end offset (number of bytes)",
                        "Selection: %1 - %2 (%3)", QLatin1String(codedSelectionStart), QLatin1String(codedSelectionEnd), bytesCount );
    }
    else
        string = i18nc( "@info:status offset value not available", "Selection: -" );

    mSelectionLabel->setText( string );
}
// #if 0
void ViewStatusController::onValueCodingChanged( int valueCoding )
{
    mValueCodingComboBox->setCurrentIndex( valueCoding );
}

void ViewStatusController::onCharCodecChanged( const QString& charCodecName )
{
    const int charCodingIndex = KHECore::CharCodec::codecNames().indexOf( charCodecName );

    mCharCodingComboBox->setCurrentIndex( charCodingIndex );
}
// #endif

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
// Okteta core
#include <charcodec.h>
#include <khe.h>
// KDE
#include <KComboBox>
#include <KStatusBar>
#include <KLocale>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QFontMetrics>

// TODO: make status bar capable to hide entries if size is too small, use priorisation

ViewStatusController::ViewStatusController( KStatusBar* statusBar )
 : mByteArrayDisplay( 0 ), mStatusBar( statusBar )
{
    mPrintFunction = KHEUI::KOffsetFormat::printFunction( KHEUI::KOffsetFormat::Hexadecimal );

    mOffsetLabel = new QLabel( statusBar );
    statusBar->addWidget( mOffsetLabel, 0 );

    mOverwriteModeLabel = new QLabel( statusBar );
    statusBar->addWidget( mOverwriteModeLabel, 0 );

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
    statusBar->addWidget( mValueCodingComboBox, 0 );

    mCharCodingComboBox = new KComboBox( statusBar );
    mCharCodingComboBox->addItems( KHECore::CharCodec::codecNames() );
    mCharCodingComboBox->setToolTip(
        i18nc("@info:tooltip","Encoding in the character column of the current view.") );
    connect( mCharCodingComboBox, SIGNAL(activated(int)), SLOT(setCharCoding(int)) );
    statusBar->addWidget( mCharCodingComboBox, 0 );

    fixWidths();

    setTargetModel( 0 );
}


void ViewStatusController::fixWidths()
{
    const QFontMetrics metrics = mStatusBar->fontMetrics();

    // mOffsetLabel
    static const char HexDigitsCount = 16;
    static const char HexDigits[HexDigitsCount] =
    {
        '0','1','2','3','4','5','6','7','8','9',
        'A','B','C','D','E','F'
    };

    int largestOffsetWidth = 0;
    for( int i=0; i<HexDigitsCount; ++i )
    {
        QString offset = QString( 9, HexDigits[i] );
        offset[4] = ':';
        const QString offsetText = i18n( "Offset: %1", offset );
        const int offsetWidth = metrics.boundingRect( offsetText ).width();
        if( largestOffsetWidth < offsetWidth )
            largestOffsetWidth = offsetWidth;
    }
    mOffsetLabel->setFixedWidth( largestOffsetWidth );

    // mOverwriteModeLabel
    const QString ovr = i18nc( "@info:status short for: Overwrite mode", "OVR" );
    const QString ins = i18nc( "@info:status short for: Insert mode",    "INS" );
    const int ovrWidth = metrics.boundingRect( ovr ).width();
    const int insWidth = metrics.boundingRect( ins ).width();
    mOverwriteModeLabel->setFixedWidth( ovrWidth>insWidth ? ovrWidth : insWidth );
}

void ViewStatusController::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<KByteArrayDisplay*>() : 0;

    const bool hasView = ( mByteArrayDisplay != 0 );
    if( hasView )
    {
        mStartOffset = mByteArrayDisplay->startOffset();

        onCursorPositionChanged( mByteArrayDisplay->cursorPosition() );
        onOverwriteModeChanged( mByteArrayDisplay->isOverwriteMode() );
        onValueCodingChanged( (int)mByteArrayDisplay->valueCoding() );
        onCharCodecChanged( mByteArrayDisplay->charCodingName() );

        connect( mByteArrayDisplay, SIGNAL(cursorPositionChanged( int )), SLOT(onCursorPositionChanged( int )) );
        connect( mByteArrayDisplay, SIGNAL(overwriteModeChanged( bool )), SLOT(onOverwriteModeChanged( bool )) );
        connect( mByteArrayDisplay, SIGNAL(valueCodingChanged( int )), SLOT(onValueCodingChanged( int )) );
        connect( mByteArrayDisplay, SIGNAL(charCodecChanged( const QString& )),
            SLOT(onCharCodecChanged( const QString& )) );
    }
    else
    {
        const QString emptyString( '-' );

        mOffsetLabel->setText( i18nc("@info:status offset value not available", "Offset: -") );
        mOverwriteModeLabel->setText( emptyString );
    }

    mOffsetLabel->setEnabled( hasView );
    mOverwriteModeLabel->setEnabled( hasView );
    mValueCodingComboBox->setEnabled( hasView );
    mCharCodingComboBox->setEnabled( hasView );
}

void ViewStatusController::setValueCoding( int valueCoding )
{
    mByteArrayDisplay->setValueCoding( valueCoding );
}

void ViewStatusController::setCharCoding( int charCoding )
{
    mByteArrayDisplay->setCharCoding( KHECore::CharCodec::codecNames()[charCoding] );
}

void ViewStatusController::onCursorPositionChanged( int offset )
{
    static char codedOffset[KHEUI::KOffsetFormat::MaxFormatWidth+1];

    mPrintFunction( codedOffset, mStartOffset + offset );

    mOffsetLabel->setText( i18n("Offset: %1", QLatin1String(codedOffset)) );
}

void ViewStatusController::onOverwriteModeChanged( bool isOverwrite )
{
    const QString overwriteModeText = isOverwrite ?
        i18nc( "@info:status short for: Overwrite mode", "OVR" ) :
        i18nc( "@info:status short for: Insert mode",    "INS");
    mOverwriteModeLabel->setText( overwriteModeText );

    const QString overwriteModeToolTip = isOverwrite ?
        i18nc( "@info:tooltip", "Overwrite mode" ) :
        i18nc( "@info:tooltip", "Insert mode" );
    mOverwriteModeLabel->setToolTip( overwriteModeToolTip );
}

void ViewStatusController::onValueCodingChanged( int valueCoding )
{
    mValueCodingComboBox->setCurrentIndex( valueCoding );
}

void ViewStatusController::onCharCodecChanged( const QString& charCodecName )
{
    const int charCodingIndex = KHECore::CharCodec::codecNames().indexOf( charCodecName );

    mCharCodingComboBox->setCurrentIndex( charCodingIndex );
}

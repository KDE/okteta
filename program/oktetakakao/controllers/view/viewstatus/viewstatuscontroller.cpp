/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

    mValueCodingLabel = new QLabel( statusBar );
    mValueCodingLabel->setToolTip(
        i18nc("@info:tooltip","Coding in the value column of the current view.") );
    statusBar->addWidget( mValueCodingLabel, 0 );

    mCharCodingLabel = new QLabel( statusBar );
    mCharCodingLabel->setToolTip(
        i18nc("@info:tooltip","Encoding in the character column of the current view.") );
    statusBar->addWidget( mCharCodingLabel, 0 );

    fixWidths();

    setTargetModel( 0 );
}


QString ViewStatusController::valueCodingName( int valueCoding ) const
{
    const QString valueCodingName =
         valueCoding == KHECore::HexadecimalCoding ?
            i18nc("@info:status encoding of the bytes as values in the hexadecimal format","Hexadecimal" ) :
         valueCoding == KHECore::DecimalCoding ?
            i18nc("@info:status encoding of the bytes as values in the decimal format",    "Decimal") :
         valueCoding == KHECore::OctalCoding ?
            i18nc("@info:status encoding of the bytes as values in the octal format",      "Octal" ) :
         valueCoding == KHECore::BinaryCoding ?
            i18nc("@info:status encoding of the bytes as values in the binary format",     "Binary") :
            QString();

    return valueCodingName;
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

    // mValueCodingLabel
    int largestValueCodingNameWidth = 0;
    for( int vc=KHECore::HexadecimalCoding; vc<=KHECore::BinaryCoding; ++vc )
    {
        const QString name = valueCodingName( vc );
        const int valueCodingNameWidth = metrics.boundingRect( name ).width();
        if( largestValueCodingNameWidth < valueCodingNameWidth )
            largestValueCodingNameWidth = valueCodingNameWidth;
    }
    mValueCodingLabel->setFixedWidth( largestValueCodingNameWidth );

    // mCharCodingLabel
    const QStringList charCodingNames = KHECore::CharCodec::codecNames();
    int largestCharCodingNameWidth = 0;
    foreach( const QString& charCodingName, charCodingNames )
    {
        const int charCodingNameWidth = metrics.boundingRect( charCodingName ).width();
        if( largestCharCodingNameWidth < charCodingNameWidth )
            largestCharCodingNameWidth = charCodingNameWidth;
    }
    mCharCodingLabel->setFixedWidth( largestCharCodingNameWidth );

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
        mValueCodingLabel->setText( emptyString );
        mCharCodingLabel->setText( emptyString );
    }

    mOffsetLabel->setEnabled( hasView );
    mOverwriteModeLabel->setEnabled( hasView );
    mValueCodingLabel->setEnabled( hasView );
    mCharCodingLabel->setEnabled( hasView );
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
    const QString name = valueCodingName( valueCoding );

    mValueCodingLabel->setText( name );
}

void ViewStatusController::onCharCodecChanged( const QString& charCodecName )
{
    mCharCodingLabel->setText( charCodecName );
}

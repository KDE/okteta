/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kprimitivetypesview.h"

// controller
#include "poddecodertool.h"
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
// KDE
#include <KLineEdit>
#include <KComboBox>
#include <KDialog>
#include <KLocale>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QStyle>


static const unsigned char PrimitivesDefaultUndefinedChar = '?';

KPrimitiveTypesView::KPrimitiveTypesView( PODDecoderTool *tool, QWidget *parent )
 : QWidget( parent ), mTool( tool ), mCharCodec( 0 ), mUndefinedChar( PrimitivesDefaultUndefinedChar )
{
    mCharCodec = KHECore::KCharCodec::createCodec( KHECore::LocalEncoding );

    // values
    QGridLayout *baseLayout = new QGridLayout( this );
    baseLayout->setMargin( 0 );//KDialog::marginHint() );
    baseLayout->setSpacing( KDialog::spacingHint() );
    baseLayout->setColumnStretch( 3, 10 );

    QGridLayout *valuesLayout = new QGridLayout();
    valuesLayout->setSpacing( KDialog::spacingHint() );
    valuesLayout->setColumnStretch( 2, 10 );
    valuesLayout->setColumnStretch( 5, 10 );

    const QString Int8Int16Label[NoOfRows] =
    {
        i18nc("@label:textbox","Signed 8 bit:"),
        i18nc("@label:textbox","Unsigned 8 bit:"),
        i18nc("@label:textbox","Signed 16 bit:"),
        i18nc("@label:textbox","Unsigned 16 bit:")
    };

    const QString Int32FloatLabel[NoOfRows] =
    {
        i18nc("@label:textbox","Signed 32 bit:"),
        i18nc("@label:textbox","Unsigned 32 bit:"),
        i18nc("@label:textbox","32 bit float:"),
        i18nc("@label:textbox","64 bit float:")
    };

    const QString ByteCodeLabel[NoOfRows] =
    {
        i18nc("@label:textbox encoding of the bytes as value in the hexadecimal format","Hexadecimal:"),
        i18nc("@label:textbox encoding of the bytes as value in the octal format",      "Octal:"),
        i18nc("@label:textbox encoding of the bytes as value in the binary format",     "Binary:"),
        i18nc("@label:textbox encoding of the bytes as character",                      "Character:")
    };

    for( int i=0; i<NoOfRows; ++i )
    {
        // first column
        QLabel *label = new QLabel( Int8Int16Label[i] );
        label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
        valuesLayout->addWidget( label, i, 0 );

        mInt8Int16Display[i] = new KLineEdit();
        mInt8Int16Display[i]->setReadOnly( true );
        mInt8Int16Display[i]->setAlignment( Qt::AlignRight );
        valuesLayout->addWidget( mInt8Int16Display[i], i, 1 );

        // second column
        label = new QLabel( Int32FloatLabel[i] );
        label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
        valuesLayout->addWidget( label, i, 2 );

        mInt32FloatDisplay[i] = new KLineEdit();
        mInt32FloatDisplay[i]->setReadOnly( true );
        mInt32FloatDisplay[i]->setAlignment( Qt::AlignRight );
        valuesLayout->addWidget( mInt32FloatDisplay[i], i, 3 );

        // third column
        label = new QLabel( ByteCodeLabel[i] );
        label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
        baseLayout->addWidget( label, i, 1 );

        mByteCodeDisplay[i] = new KLineEdit();
        mByteCodeDisplay[i]->setReadOnly( true );
        mByteCodeDisplay[i]->setAlignment( Qt::AlignRight );
        baseLayout->addWidget( mByteCodeDisplay[i], i, 2 );
    }

    // settings
    QBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->setSpacing( KDialog::spacingHint() );
    settingsLayout->setMargin( 0 );

    mEndianSelection = new KComboBox();
    mEndianSelection->addItem( i18nc("@item:inlistbox","Little endian") ); // add first for index
    mEndianSelection->addItem( i18nc("@item:inlistbox","Big endian") );    // add second for index
    mEndianSelection->setCurrentIndex( mTool->podData().byteOrder() );
    settingsLayout->addWidget( mEndianSelection, 0, Qt::AlignVCenter );
    connect( mEndianSelection, SIGNAL(activated(int)), SLOT(onEndianActivation(int)));

    mUnsignedAsHexCheck = new QCheckBox( i18nc("@option:check","Show unsigned as hexadecimal") );
    settingsLayout->addWidget( mUnsignedAsHexCheck, 0, Qt::AlignVCenter );
    connect( mUnsignedAsHexCheck, SIGNAL(clicked()), this, SLOT(onUnsignedAsHex()) );

    // Variable bitwidth
//     QLabel *label = new QLabel( i18n("Stream length:") );
//     label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
//     baseLayout->addWidget( label, 4, 1 );

//     mBitWidthSelection = new KComboBox( false );
//     mBitWidthSelection->addItem( i18n("Fixed 8 bit") );
//     for( int i=1; i<=16; ++i )
//         mBitWidthSelection->addItem( i18np("1 bit window","%1 bits window",i) );
//     baseLayout->addWidget( mBitWidthSelection, 4, 2 );
//     connect( mBitWidthSelection, SIGNAL(activated(int)), SLOT(onBitWidthChange(int)));

    // load font metrics
    fontChanged();

    baseLayout->addLayout( valuesLayout,   0, 0, 4, 1, Qt::AlignLeft|Qt::AlignVCenter );
    baseLayout->addLayout( settingsLayout, 4, 0, Qt::AlignLeft|Qt::AlignVCenter );

    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()), SLOT(fontChanged()) );

//TODO: remove if signal works again
//     connect( mTool, SIGNAL(dataChanged( const unsigned char* )), SLOT(onDataChange( const unsigned char* )) );
    mTool->setPODView( this );

    setMinimumSize( sizeHint() );
}

void KPrimitiveTypesView::setCharCode( const QString &codeName )
{
    if( codeName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codeName );
    onDataChange();
}


void KPrimitiveTypesView::setUndefinedChar( const QChar &undefinedChar )
{
    if( mUndefinedChar == undefinedChar )
        return;

    mUndefinedChar = undefinedChar;
    onDataChange();
}


void KPrimitiveTypesView::onDataChange()
{
    static const int HexBase = 16;
    static const int OctBase = 8;
    static const int DualBase = 2;
    const QLatin1Char ZeroChar('0');
    const QString EmptyNote( '-' );
    const QString hexMask = QString::fromLatin1( "0x%1" );
    const bool noHex = !mUnsignedAsHexCheck->isChecked();

    const void *P8Bit, *P16Bit, *P32Bit, *P64Bit;

    const PODData &podData = mTool->podData();
    podData.pointers( &P8Bit, &P16Bit, &P32Bit, &P64Bit );

    // unsigned 8 bit
    const QString unsigned8Bit = (P8Bit == 0) ? EmptyNote :
                                 noHex ?        QString::number(*(unsigned char*)P8Bit) :
                                                hexMask.arg(*(unsigned char*)P8Bit,2,HexBase,ZeroChar);
    mInt8Int16Display[1]->setText( unsigned8Bit );

    // signed int 8 bit
    const QString signed8Bit = (P8Bit == 0) ? EmptyNote :
                                              QString::number(*(signed char*)P8Bit);
    mInt8Int16Display[0]->setText( signed8Bit );

    // unsigned int 16 bit
    const QString unsigned16Bit = (P16Bit == 0) ? EmptyNote :
                                  noHex ?         QString::number(*(unsigned short*)P16Bit) :
                                                  hexMask.arg(*(unsigned short*)P16Bit,4,HexBase,ZeroChar);
    mInt8Int16Display[3]->setText( unsigned16Bit );

    // signed int 16 bit
    const QString signed16Bit = (P16Bit == 0) ? EmptyNote :
                                                QString::number(*(signed short*)P16Bit);
    mInt8Int16Display[2]->setText( signed16Bit );

    // unsigned int 32 bit
    const QString unsigned32Bit = (P32Bit == 0) ? EmptyNote :
                                  noHex ?         QString::number(*(unsigned int*)P32Bit) :
                                                  hexMask.arg(*(unsigned int*)P32Bit,8,HexBase,ZeroChar);
    mInt32FloatDisplay[1]->setText( unsigned32Bit );

    // signed int 32 bit
    const QString signed32Bit = (P32Bit == 0) ? EmptyNote :
                                                QString::number(*(signed int*)P32Bit);
    mInt32FloatDisplay[0]->setText( signed32Bit );

    // float 32 bit
    const QString float32Bit = (P32Bit == 0) ? EmptyNote :
                                               QString::number(*(float*)P32Bit,'e');
    mInt32FloatDisplay[2]->setText( float32Bit );

    // float 64 bit
    const QString double64Bit = (P64Bit == 0) ? EmptyNote :
                                                QString::number(*(double*)P64Bit,'e');
    mInt32FloatDisplay[3]->setText( double64Bit );

    const int numBits = 0;//mBitWidthSelection->currentIndex();
    const bool hasEnoughBits = ( (numBits>8 ? P16Bit : P8Bit) != 0 );
    if( hasEnoughBits )
    {
        const QString SimpleMask = QString::fromLatin1( "%1" );
        if( numBits <= 0 )
        {
            const unsigned char byte = *(unsigned char*)P8Bit;
            mByteCodeDisplay[0]->setText( SimpleMask.arg(byte,2,HexBase,ZeroChar) );
            mByteCodeDisplay[1]->setText( SimpleMask.arg(byte,3,OctBase,ZeroChar) );
            mByteCodeDisplay[2]->setText( SimpleMask.arg(byte,8,DualBase,ZeroChar) );
        }
        else
        {
            const unsigned long bits = podData.bitValue( numBits );
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

            const unsigned int lowerBits = bits&0xFF;
            mByteCodeDisplay[0]->setText( HexDezValue.arg(lowerBits,2,HexBase,ZeroChar) );
            mByteCodeDisplay[1]->setText( OktalValue.arg(lowerBits,3,OctBase,ZeroChar) );

            mByteCodeDisplay[2]->setText( SimpleMask.arg(bits,numBits,DualBase,ZeroChar) );
        }

        const KHECore::KChar decodedChar = mCharCodec->decode( *(unsigned char*)P8Bit );
        mByteCodeDisplay[3]->setText( QString(decodedChar.isUndefined()?mUndefinedChar:(QChar)decodedChar) );
    }
    else
        for( int i=0; i<NoOfRows; ++i )
            mByteCodeDisplay[i]->setText( EmptyNote );
}


void KPrimitiveTypesView::fontChanged()
{
    KLineEdit* exampleLineEdit = mInt8Int16Display[0];

    const int verticalMargin = 1; // hardcoded in gui/widgets/qlineedit.cpp
    const int horizontalMargin = 2; // hardcoded in gui/widgets/qlineedit.cpp
    const int mysticBonusMargin = 1; // no idea where it come from, but with one pixel more it is equally margined
    int leftMargin, rightMargin, topMargin, bottomMargin;
    exampleLineEdit->getContentsMargins( &leftMargin, &topMargin, &rightMargin, &bottomMargin );

    const QFontMetrics fontMetrics( font() );
    const int Int8Int16Width = fontMetrics.width( "888888" );
    const int Int32FloatWidth = fontMetrics.width( "88.888888e8888" );
    const int ByteCodeWidth = fontMetrics.width( "88888888" ); // max 16 bits had "888888888888888888"
    const int lineSpacing = fontMetrics.lineSpacing();

    const int minLineSpacing = 14;
    const int contentHeight =
        qMax(lineSpacing,minLineSpacing)
        + topMargin + 2 * verticalMargin + bottomMargin;
    const int contentOtherWidth = leftMargin + 2 * horizontalMargin + rightMargin + mysticBonusMargin;

    const QSize Int8Int16ContentSize( Int8Int16Width + contentOtherWidth, contentHeight );
    const QSize Int32FloatContentSize( Int32FloatWidth + contentOtherWidth, contentHeight );
    const QSize ByteCodeContentSize( ByteCodeWidth + contentOtherWidth, contentHeight );

    QStyleOptionFrameV2 option;
    option.initFrom( exampleLineEdit );
    option.lineWidth = exampleLineEdit->hasFrame() ?
         style()->pixelMetric( QStyle::PM_DefaultFrameWidth, &option, exampleLineEdit ) : 0;

    const int Int8Int16DisplayWidth =
        style()->sizeFromContents( QStyle::CT_LineEdit, &option, Int8Int16ContentSize, exampleLineEdit ).width();
    const int Int32FloatDisplayWidth =
        style()->sizeFromContents( QStyle::CT_LineEdit, &option, Int32FloatContentSize, exampleLineEdit ).width();
    const int ByteCodeDisplayWidth =
        style()->sizeFromContents( QStyle::CT_LineEdit, &option, ByteCodeContentSize, exampleLineEdit ).width();

    for( int i=0; i<NoOfRows; ++i )
    {
        mInt8Int16Display[i]->setFixedWidth( Int8Int16DisplayWidth );
        mInt32FloatDisplay[i]->setFixedWidth( Int32FloatDisplayWidth );
        mByteCodeDisplay[i]->setFixedWidth( ByteCodeDisplayWidth );
    }
}


void KPrimitiveTypesView::onEndianActivation( int index )
{
    mTool->podData().setByteOrder( index );
    onDataChange();
}


void KPrimitiveTypesView::onUnsignedAsHex()
{
    onDataChange();
}


void KPrimitiveTypesView::onBitWidthChange( int )
{
    onDataChange();
}

KPrimitiveTypesView::~KPrimitiveTypesView()
{
    delete mCharCodec;
}

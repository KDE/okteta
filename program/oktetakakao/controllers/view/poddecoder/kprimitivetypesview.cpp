/***************************************************************************
                          kprimitivetypesview.cpp -  description
                             -------------------
    begin                : Thu Nov 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kprimitivetypesview.h"

// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
// KDE
#include <KDialog>
#include <KLocale>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>


static const unsigned char DefaultUndefinedChar = '?';

static const int LittleEndianIndex = 0; // Intel, Alpha, ...
static const int BigEndianIndex =    1; // Sun, Motorola, ...
// TODO: add PDP endianess
static const int ThisMachineEndianIndex =
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
  LittleEndianIndex;
#else
  BigEndianIndex;
#endif


KPrimitiveTypesView::KPrimitiveTypesView( QWidget *Parent )
 : QWidget( Parent ), ByteArray( 0 ), CharCodec( 0 ), UndefinedChar( DefaultUndefinedChar )
{
    CharCodec = KHECore::KCharCodec::createCodec( KHECore::LocalEncoding );

  // values
  QGridLayout *BaseLayout = new QGridLayout( this );
  BaseLayout->setMargin( KDialog::marginHint() );
  BaseLayout->setSpacing( KDialog::spacingHint() );
  BaseLayout->setColumnStretch( 3, 10 );

  QGridLayout *ValuesLayout = new QGridLayout();
  ValuesLayout->setSpacing( KDialog::spacingHint() );
  ValuesLayout->setColumnStretch( 2, 10 );
  ValuesLayout->setColumnStretch( 5, 10 );

  const QString Int8Int16Label[4] =
  {
    i18n("Signed 8 bit:"), i18n("Unsigned 8 bit:"),
    i18n("Signed 16 bit:"), i18n("Unsigned 16 bit:")
  };

  const QString Int32FloatLabel[4] =
  {
    i18n("Signed 32 bit:"), i18n("Unsigned 32 bit:"),
    i18n("32 bit float:"), i18n("64 bit float:")
  };

  const QString ByteCodeLabel[4] =
  {
    i18n("Hexadecimal:"), i18n("Octal:"),
    i18n("Binary:"), i18n("Text:")
  };

  for( int i=0; i<4; ++i )
  {
    // first column
    QLabel *Label = new QLabel( Int8Int16Label[i] );
    Label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
    ValuesLayout->addWidget( Label, i, 0 );

    Int8Int16Display[i] = new QLineEdit();
    Int8Int16Display[i]->setReadOnly( true );
    Int8Int16Display[i]->setAlignment( Qt::AlignRight );
    ValuesLayout->addWidget( Int8Int16Display[i], i, 1 );

    // second column
    Label = new QLabel( Int32FloatLabel[i] );
    Label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
    ValuesLayout->addWidget( Label, i, 2 );

    Int32FloatDisplay[i] = new QLineEdit();
    Int32FloatDisplay[i]->setReadOnly( true );
    Int32FloatDisplay[i]->setAlignment( Qt::AlignRight );
    ValuesLayout->addWidget( Int32FloatDisplay[i], i, 3 );

    // third column
    Label = new QLabel( ByteCodeLabel[i] );
    Label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
    BaseLayout->addWidget( Label, i, 1 );

    ByteCodeDisplay[i] = new QLineEdit();
    ByteCodeDisplay[i]->setReadOnly( true );
    ByteCodeDisplay[i]->setAlignment( Qt::AlignRight );
    BaseLayout->addWidget( ByteCodeDisplay[i], i, 2 );
  }

    // settings
    QBoxLayout * SettingsLayout = new QHBoxLayout();
    SettingsLayout->setSpacing( KDialog::spacingHint() );
    SettingsLayout->setMargin( 0 );

    EndianSelection = new QComboBox();
    EndianSelection->addItem( i18n("Little endian") ); // add first for index
    EndianSelection->addItem( i18n("Big endian") );    // add second for index
    EndianSelection->setCurrentIndex( ThisMachineEndianIndex );
    SettingsLayout->addWidget( EndianSelection, 0, Qt::AlignVCenter );
    connect( EndianSelection, SIGNAL(activated(int)), SLOT(onEndianActivation(int)));

    UnsignedAsHexCheck = new QCheckBox( i18n("Show unsigned as hexadecimal") );
    SettingsLayout->addWidget( UnsignedAsHexCheck, 0, Qt::AlignVCenter );
    connect( UnsignedAsHexCheck, SIGNAL(clicked()), this, SLOT(onUnsignedAsHex()) );

    // Variable bitwidth
    QLabel *Label = new QLabel( i18n("Stream length:") );
    Label->setAlignment( Qt::AlignRight|Qt::AlignVCenter );
    BaseLayout->addWidget( Label, 4, 1 );

    BitWidthSelection = new QComboBox( false );
    BitWidthSelection->addItem( i18n("Fixed 8 bit") );
    for( int i=1; i<=16; ++i )
        BitWidthSelection->addItem( i18np("1 bit window","%n bits window",i) );
    BaseLayout->addWidget( BitWidthSelection, 4, 2 );
    connect( BitWidthSelection, SIGNAL(activated(int)), SLOT(onBitWidthChange(int)));

    // load font metrics
    fontChanged();

    BaseLayout->addLayout( ValuesLayout,   0, 0, 4, 1, Qt::AlignLeft|Qt::AlignVCenter );
    BaseLayout->addLayout( SettingsLayout, 4, 0, Qt::AlignLeft|Qt::AlignVCenter );

    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()), SLOT(fontChanged()) );

    setMinimumSize( sizeHint() );
}

void KPrimitiveTypesView::setCharCode( const QString &CodeName )
{
    if( CodeName == CharCodec->name() )
        return;

    delete CharCodec;
    CharCodec = KHECore::KCharCodec::createCodec( CodeName );
    onByteArrayChange( ByteArray );
}


void KPrimitiveTypesView::setUndefinedChar( const QChar &Char )
{
    if( UndefinedChar == Char )
        return;

    UndefinedChar = Char;
    onByteArrayChange( ByteArray );
}


unsigned long bitValue( const unsigned char *ByteArray, int NoOfBitsToRead )
{
    static const unsigned char BitMask[9] =
    {
        0, 1<<7, 3<<6, 7<<5, 15<<4, 31<<3, 63<<2, 127<<1, 255
    };

    unsigned long Result = 0;

    int NoOfUsedBits = 0;//7 - state.cell;

    while( NoOfBitsToRead > 0 )
    {
        unsigned char Byte = *ByteArray << NoOfUsedBits;

        const int NoOfNextBits = ( (8-NoOfUsedBits) >= NoOfBitsToRead ) ? NoOfBitsToRead : (8-NoOfUsedBits);

        Byte &= BitMask[NoOfNextBits];

        Byte >>= 8 - NoOfNextBits;

        Result = (Result<<NoOfNextBits) | Byte;

        NoOfBitsToRead -= NoOfNextBits;
        NoOfUsedBits += NoOfNextBits;

        if( NoOfUsedBits >= 8 )
        {
            NoOfUsedBits = 0;
            ++ByteArray;
        }
    }

    return( Result );
}


void KPrimitiveTypesView::onByteArrayChange( const unsigned char *B )
{
    ByteArray = B;

    if( !ByteArray )
    {
        const QString EmptyNote( '-' );
        for( int i=0; i<4; ++i )
        {
            Int8Int16Display[i]->setText( EmptyNote );
            Int32FloatDisplay[i]->setText( EmptyNote );
            ByteCodeDisplay[i]->setText( EmptyNote );
        } //TODO: grey fields out
    }
    else
    {
        void *P8Bit, *P16Bit, *P32Bit, *P64Bit;
        // ensure strict alignment for double as needed on some architectures (e.g. PA-RISC)
        typedef union { unsigned char Data[8]; double Dummy; } KAligned64Bit;
        KAligned64Bit Aligned64Bit;
        if( EndianSelection->currentIndex() == ThisMachineEndianIndex )
        {
            // take it as it is
            memcpy( Aligned64Bit.Data, ByteArray, 8 );
            P8Bit = P16Bit = P32Bit = P64Bit = Aligned64Bit.Data;
        }
        else
        {
            // reverse order
            for( int i=0,j=7; i<8; ++i,--j )
                Aligned64Bit.Data[i] = ByteArray[j];

            P8Bit =  &Aligned64Bit.Data[7];
            P16Bit = &Aligned64Bit.Data[6];
            P32Bit = &Aligned64Bit.Data[4];
            P64Bit = Aligned64Bit.Data;
        }

        const bool NoHex = !UnsignedAsHexCheck->isChecked();
        const QString HexMask = QString::fromLatin1( "0x%1" );
        static const int HexBase = 16;
        static const int OctBase = 8;
        static const int DualBase = 2;
        const QLatin1Char ZeroChar('0');

        // unsigned 8 bit
        const unsigned char Unsigned8Bit = *(unsigned char*)P8Bit;
        Int8Int16Display[1]->setText( NoHex ? QString::number(Unsigned8Bit) :
                                  HexMask.arg(Unsigned8Bit,2,HexBase,ZeroChar) );
        // signed int 8 bit
        const signed char Signed8Bit = *(signed char*)P8Bit;
        Int8Int16Display[0]->setText( QString::number(Signed8Bit) );

        // unsigned int 16 bit
        const unsigned short Unsigned16Bit = *(unsigned short*)P16Bit;
        Int8Int16Display[3]->setText( NoHex ? QString::number(Unsigned16Bit) :
                                  HexMask.arg(Unsigned16Bit,4,HexBase,ZeroChar) );
        // signed int 16 bit
        const signed short Signed16Bit = *(signed short*)P16Bit;
        Int8Int16Display[2]->setText( QString::number(Signed16Bit) );

        // unsigned int 32 bit
        const unsigned int Unsigned32Bit = *(unsigned int*)P32Bit;
        Int32FloatDisplay[1]->setText( NoHex ? QString::number(Unsigned32Bit) :
                                  HexMask.arg(Unsigned32Bit,8,HexBase,ZeroChar) );
        // signed int 32 bit
        const signed int Signed32Bit = *(signed int*)P32Bit;
        Int32FloatDisplay[0]->setText( QString::number(Signed32Bit) );

        // float 32 bit
        const float Float32Bit = *(float*)P32Bit;
        Int32FloatDisplay[2]->setText( QString::number(Float32Bit,'e') );
        // float 64 bit
        const double Double64Bit = *(double*)P64Bit;
        Int32FloatDisplay[3]->setText( QString::number(Double64Bit,'e') );

        const QString SimpleMask = QString::fromLatin1( "%1" );
        int numBits = BitWidthSelection->currentIndex();
        if( numBits <= 0 )
        {
            unsigned char Byte = (unsigned char)ByteArray[0];
            ByteCodeDisplay[0]->setText( SimpleMask.arg(Byte,2,HexBase,ZeroChar) );
            ByteCodeDisplay[1]->setText( SimpleMask.arg(Byte,3,OctBase,ZeroChar) );
            ByteCodeDisplay[2]->setText( SimpleMask.arg(Byte,8,DualBase,ZeroChar) );
        }
        else
        {
            unsigned long Bits = bitValue( ByteArray, numBits );
            const QString MoreThan8BitMask = QString::fromLatin1( "%1 %2" );
            QString HexDezValue;
            QString OktalValue;
            if( numBits > 8 )
            {
                const unsigned int HigherBits = (Bits>>8)&0xFF;
                HexDezValue = MoreThan8BitMask.arg( HigherBits, 2, HexBase, ZeroChar );
                OktalValue = MoreThan8BitMask.arg( HigherBits, 3, OctBase, ZeroChar );
            }
            else
            { HexDezValue = SimpleMask; OktalValue = SimpleMask; }

            const unsigned int LowerBits = Bits&0xFF;
            ByteCodeDisplay[0]->setText( HexDezValue.arg(LowerBits,2,HexBase,ZeroChar) );
            ByteCodeDisplay[1]->setText( OktalValue.arg(LowerBits,3,OctBase,ZeroChar) );

            ByteCodeDisplay[2]->setText( SimpleMask.arg(Bits,numBits,DualBase,ZeroChar) );
        }

        const KHECore::KChar Char = CharCodec->decode( ByteArray[0] );
        ByteCodeDisplay[3]->setText( QString(Char.isUndefined()?UndefinedChar:(QChar)Char) );
    }
}


void KPrimitiveTypesView::fontChanged()
{
    QFontMetrics metric( font() );
    const int Int8Int16Width = metric.width( "XXXXXXXX" );
    const int Int32FloatWidth = metric.width( "XXXXXXXXXXXX" );
    const int ByteCodeWidth = metric.width( "888888888888888888" );

    for( int i=0; i<4; ++i )
    {
        Int8Int16Display[i]->setFixedWidth( Int8Int16Width );
        Int32FloatDisplay[i]->setFixedWidth( Int32FloatWidth );
        ByteCodeDisplay[i]->setFixedWidth( ByteCodeWidth );
    }
}


void KPrimitiveTypesView::onEndianActivation( int )
{
    onByteArrayChange( ByteArray );
}


void KPrimitiveTypesView::onUnsignedAsHex()
{
    onByteArrayChange( ByteArray );
}


void KPrimitiveTypesView::onBitWidthChange( int )
{
    onByteArrayChange( ByteArray );
}

KPrimitiveTypesView::~KPrimitiveTypesView()
{
    delete CharCodec;
}

#include "kprimitivetypesview.moc"

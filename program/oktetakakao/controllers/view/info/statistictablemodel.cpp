/***************************************************************************
                          statistictablemodel.cpp  -  description
                             -------------------
    begin                : Wed Jan 2 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "statistictablemodel.h"

// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <valuecodec.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QLatin1Char>


static const unsigned char DefaultUndefinedChar = '?';
static const KHECore::KCoding DefaultValueCoding =  KHECore::HexadecimalCoding;
static const int ByteSetSize = 256;


StatisticTableModel::StatisticTableModel( int *byteCount, QObject *parent )
 : QAbstractTableModel( parent ),
   mByteCount( byteCount ),
   mValueCoding( DefaultValueCoding ),
   mValueCodec( KHECore::ValueCodec::createCodec(DefaultValueCoding) ),
   mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( DefaultUndefinedChar )
{
}

void StatisticTableModel::update( int size )
{
    mSize = size;
    emit dataChanged( index(0,CountId), index(ByteSetSize-1,PercentId) );
    emit sizeChanged( mSize );
}

void StatisticTableModel::setUndefinedChar( const QChar &undefinedChar )
{
    mUndefinedChar = undefinedChar;

    emit dataChanged( index(0,CharacterId), index(ByteSetSize-1,CharacterId) );
}

void StatisticTableModel::setValueCoding( int coding )
{
    // no changes?
    if( mValueCoding == coding )
        return;

    delete mValueCodec;

    mValueCoding = (KHECore::KCoding)coding;
    mValueCodec = KHECore::ValueCodec::createCodec( mValueCoding );
//     CodedByte.resize( ByteCodec->encodingWidth() );

    emit dataChanged( index(0,ValueId), index(ByteSetSize-1,ValueId) );
    emit headerChanged();
}

void StatisticTableModel::setCharCodec( const QString &codeName )
{
    if( codeName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codeName );

    emit dataChanged( index(0,CharacterId), index(ByteSetSize-1,CharacterId) );
}


int StatisticTableModel::rowCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return ByteSetSize;
}

int StatisticTableModel::columnCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return NoOfIds;
}

QVariant StatisticTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        const unsigned char byte = index.row();
        const int column = index.column();
        switch( column )
        {
            case CharacterId:
            {
                const KHECore::KChar decodedChar = mCharCodec->decode( byte );
                result = decodedChar.isUndefined() ?
                    i18nc( "character is not defined", "undef." ) :
                    QString( (QChar)decodedChar );
                break;
            }
            case ValueId:
            {
                QString value;
                mValueCodec->encode( value, 0, byte );
                result = value;
                break;
            }
            case CountId:
                result = mByteCount[byte];
                break;
            case PercentId:
                result = ( mSize > 0 ) ?
                          // TODO: before we printed only a string (which killed sorting) with QString::number( x, 'f', 6 )
                          // Qt now cuts trailing 0s, results in unaligned numbers, not so beautiful.
                          QVariant( 100.0*(double)mByteCount[byte]/mSize ) :
                          QVariant( QLatin1Char('-') );
                break;
            default:
                ;
        }
    }
    else if( role == Qt::TextAlignmentRole )
        result = Qt::AlignRight;

    return result;
}

QVariant StatisticTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == ValueId ? (
                mValueCoding == KHECore::HexadecimalCoding ? i18nc("short for Hexadecimal","Hex") :
                mValueCoding == KHECore::DecimalCoding ?     i18nc("short for Decimal",    "Dec") :
                mValueCoding == KHECore::OctalCoding ?       i18nc("short for Octal",      "Oct") :
                mValueCoding == KHECore::BinaryCoding ?      i18nc("short for Binary",     "Bin") :
                                                QString() ) :
            section == CharacterId ?   i18nc("short for Character",      "Char") :
            section == CountId ?       i18nc("count of characters",      "Count") :
            section == PercentId ?     i18nc("Percent of byte in total", "Percent") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == ValueId ? (
                mValueCoding == KHECore::HexadecimalCoding ? i18n("Hexadecimal") :
                mValueCoding == KHECore::DecimalCoding ?     i18n("Decimal") :
                mValueCoding == KHECore::OctalCoding ?       i18n("Octal") :
                mValueCoding == KHECore::BinaryCoding ?      i18n("Binary") :
                                                QString() ) :
            section == CharacterId ?   i18n("Character") :
//             section == CountId ?       i18nc("count of characters",      "Count") :
//             section == PercentId ?     i18nc("Percent of byte in total", "Percent") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

StatisticTableModel::~StatisticTableModel()
{
    delete mValueCodec;
    delete mCharCodec;
}

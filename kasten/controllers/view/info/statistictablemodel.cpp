/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#include "statistictablemodel.h"

// Okteta core
#include <character.h>
#include <charcodec.h>
#include <valuecodec.h>
// KDE
#include <KLocale>
#include <KApplication>
#include <KColorScheme>


namespace Kasten2
{

static const unsigned char StatisticsDefaultUndefinedChar = '?';
static const Okteta::ValueCoding DefaultValueCoding =  Okteta::HexadecimalCoding;
static const int StatisticsByteSetSize = 256;


StatisticTableModel::StatisticTableModel( int *byteCount, QObject *parent )
 : QAbstractTableModel( parent ),
   mByteCount( byteCount ),
   mValueCoding( DefaultValueCoding ),
   mValueCodec( Okteta::ValueCodec::createCodec(DefaultValueCoding) ),
   mCharCodec( Okteta::CharCodec::createCodec(Okteta::LocalEncoding) ),
   mUndefinedChar( QLatin1Char(StatisticsDefaultUndefinedChar) )
{
}

void StatisticTableModel::update( int size )
{
    mSize = size;
    emit dataChanged( index(0,CountId), index(StatisticsByteSetSize-1,PercentId) );
    emit sizeChanged( mSize );
}

void StatisticTableModel::setUndefinedChar( QChar undefinedChar )
{
    mUndefinedChar = undefinedChar;

    emit dataChanged( index(0,CharacterId), index(StatisticsByteSetSize-1,CharacterId) );
}

void StatisticTableModel::setValueCoding( int valueCoding )
{
    // no changes?
    if( mValueCoding == valueCoding )
        return;

    delete mValueCodec;

    mValueCoding = (Okteta::ValueCoding)valueCoding;
    mValueCodec = Okteta::ValueCodec::createCodec( mValueCoding );
//     CodedByte.resize( ByteCodec->encodingWidth() );

    emit dataChanged( index(0,ValueId), index(StatisticsByteSetSize-1,ValueId) );
    emit headerChanged();
}

void StatisticTableModel::setCharCodec( const QString &codeName )
{
    if( codeName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = Okteta::CharCodec::createCodec( codeName );

    emit dataChanged( index(0,CharacterId), index(StatisticsByteSetSize-1,CharacterId) );
}


int StatisticTableModel::rowCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? StatisticsByteSetSize : 0;
}

int StatisticTableModel::columnCount( const QModelIndex &parent ) const
{
    return (! parent.isValid()) ? NoOfIds : 0;
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
                const Okteta::Character decodedChar = mCharCodec->decode( byte );
                result = decodedChar.isUndefined() ?
                    i18nc( "@item:intable character is not defined", "undef." ) :
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
                result =  ( mSize == -1 ) ?
                    QVariant( QLatin1String("-") ) :
                    QVariant( mByteCount[byte] );
                break;
            case PercentId:
                result = ( mSize > 0 ) ?
                          // TODO: before we printed only a string (which killed sorting) with QString::number( x, 'f', 6 )
                          // Qt now cuts trailing 0s, results in unaligned numbers, not so beautiful.
                          QVariant( 100.0*(double)mByteCount[byte]/mSize ) :
                          QVariant( QLatin1String("-") );
                break;
            default:
                ;
        }
    }
    else if( role == Qt::TextAlignmentRole )
        result = Qt::AlignRight;
    else if( role == Qt::ForegroundRole )
    {
        const int column = index.column();
        bool isInactive = false;
        switch( column )
        {
        case CountId:
            isInactive = ( mSize == -1 );
            break;
        case PercentId:
            isInactive = ( mSize <= 0 );
            break;
        default:
            ;
        }
        if( isInactive )
        {
            const QPalette& palette = KApplication::kApplication()->palette();
            const KColorScheme colorScheme( palette.currentColorGroup(), KColorScheme::View );
            result = colorScheme.foreground( KColorScheme::InactiveText );
        }
    }

    return result;
}

QVariant StatisticTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == ValueId ? (
                mValueCoding == Okteta::HexadecimalCoding ? i18nc("@title:column short for Hexadecimal","Hex") :
                mValueCoding == Okteta::DecimalCoding ?     i18nc("@title:column short for Decimal",    "Dec") :
                mValueCoding == Okteta::OctalCoding ?       i18nc("@title:column short for Octal",      "Oct") :
                mValueCoding == Okteta::BinaryCoding ?      i18nc("@title:column short for Binary",     "Bin") :
                                                QString() ) :
            section == CharacterId ?   i18nc("@title:column short for Character",      "Char") :
            section == CountId ?       i18nc("@title:column count of characters",      "Count") :
            section == PercentId ?     i18nc("@title:column Percent of byte in total", "Percent") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == ValueId ? (
                mValueCoding == Okteta::HexadecimalCoding ?
                    i18nc("@info:tooltip column contains the value in hexadecimal format","Hexadecimal") :
                mValueCoding == Okteta::DecimalCoding ?
                    i18nc("@info:tooltip column contains the value in decimal format",    "Decimal") :
                mValueCoding == Okteta::OctalCoding ?
                    i18nc("@info:tooltip column contains the value in octal format",      "Octal") :
                mValueCoding == Okteta::BinaryCoding ?
                    i18nc("@info:tooltip column contains the value in binary format",     "Binary") :
                // else
                    QString() ) :
            section == CharacterId ?
                    i18nc("@info:tooltip column contains the character with the value",   "Character") :
//             section == CountId ?       i18nc("@info:tooltip count of characters",      "Count") :
//             section == PercentId ?     i18nc("@info:tooltip Percent of byte in total", "Percent") :
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

}

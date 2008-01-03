/***************************************************************************
                          bytetablemodel.cpp  -  description
                             -------------------
    begin                : Mon Dec 31 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "bytetablemodel.h"

// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytecodec.h>
// KDE
#include <KLocale>


static const unsigned char DefaultUndefinedChar = '?';
static const int ByteSetSize = 256;

ByteTableModel::ByteTableModel( QObject *parent )
 : QAbstractTableModel( parent ),
   mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( DefaultUndefinedChar )
{
    static const KHECore::KCoding CodingIds[NofOfValueCodings] =
    {
        KHECore::DecimalCoding,
        KHECore::HexadecimalCoding,
        KHECore::OctalCoding,
        KHECore::BinaryCoding
    };
    for( int i=0; i<NofOfValueCodings; ++i )
        mValueCodec[i] = KHECore::KByteCodec::createCodec( CodingIds[i] );
}

void ByteTableModel::setUndefinedChar( const QChar &undefinedChar )
{
    mUndefinedChar = undefinedChar;

    emit dataChanged( index(0,CharacterId), index(ByteSetSize-1,CharacterId) );
}

void ByteTableModel::setCharCodec( const QString &codeName )
{
    if( codeName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codeName );

    emit dataChanged( index(0,CharacterId), index(ByteSetSize-1,CharacterId) );
}

int ByteTableModel::rowCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return ByteSetSize;
}

int ByteTableModel::columnCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return NoOfIds;
}

QVariant ByteTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        QString content;

        const unsigned char byte = index.row();
        const int column = index.column();
        if( column == CharacterId )
        {
            const KHECore::KChar decodedChar = mCharCodec->decode( byte );
            content = decodedChar.isUndefined() ?
                i18nc( "character is not defined", "undef." ) :
                QString( (QChar)decodedChar );
        }
        else if( column < CharacterId )
            mValueCodec[column]->encode( content, 0, byte );

        result = content;
    }
    else if( role == Qt::TextAlignmentRole )
        result = Qt::AlignRight;

    return result;
}

QVariant ByteTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    QVariant result;

    if( role == Qt::DisplayRole )
    {
        const QString titel =
            section == DecimalId ?     i18nc("short for Decimal",    "Dec") :
            section == HexadecimalId ? i18nc("short for Hexadecimal","Hex") :
            section == OctalId ?       i18nc("short for Octal",      "Oct") :
            section == BinaryId ?      i18nc("short for Binary",     "Bin") :
            section == CharacterId ?   i18nc("short for Character",  "Char") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == DecimalId ?     i18n("Decimal") :
            section == HexadecimalId ? i18n("Hexadecimal") :
            section == OctalId ?       i18n("Octal") :
            section == BinaryId ?      i18n("Binary") :
            section == CharacterId ?   i18n("Character") :
            QString();
        result = titel;
    }
    else
        result = QAbstractTableModel::headerData( section, orientation, role );

    return result;
}

ByteTableModel::~ByteTableModel()
{
    for( int i=0; i<NofOfValueCodings; ++i )
        delete mValueCodec[i];
    delete mCharCodec;
}

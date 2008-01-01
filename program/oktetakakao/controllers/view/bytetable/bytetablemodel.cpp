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

ByteTableModel::ByteTableModel( QObject *parent )
 : QAbstractTableModel( parent ),
   mDecimalCodec( KHECore::KByteCodec::createCodec(KHECore::DecimalCoding) ),
   mHexadecimalCodec( KHECore::KByteCodec::createCodec(KHECore::HexadecimalCoding) ),
   mOctalCodec( KHECore::KByteCodec::createCodec(KHECore::OctalCoding) ),
   mBinaryCodec( KHECore::KByteCodec::createCodec(KHECore::BinaryCoding) ),
   mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( DefaultUndefinedChar )
{
}

void ByteTableModel::setUndefinedChar( const QChar &undefinedChar )
{
    mUndefinedChar = undefinedChar;

    emit dataChanged( index(0,CharacterId), index(255,CharacterId) );
}

void ByteTableModel::setCharCodec( const QString &codeName )
{
    if( codeName == mCharCodec->name() )
        return;

    delete mCharCodec;
    mCharCodec = KHECore::KCharCodec::createCodec( codeName );

    emit dataChanged( index(0,CharacterId), index(255,CharacterId) );
}

int ByteTableModel::rowCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return 256;
}

int ByteTableModel::columnCount( const QModelIndex &parent ) const
{
Q_UNUSED( parent )
    return 5;
}

QVariant ByteTableModel::data( const QModelIndex &index, int role ) const
{
    QVariant result;
    if( role == Qt::DisplayRole )
    {
        QString content;

        const unsigned char byte = index.row();
        switch( index.column() )
        {
        case DecimalId:
            mDecimalCodec->encode( content, 0, byte );
            break;
        case HexadecimalId:
            mHexadecimalCodec->encode( content, 0, byte );
            break;
        case OctalId:
            mOctalCodec->encode( content, 0, byte );
            break;
        case BinaryId:
            mBinaryCodec->encode( content, 0, byte );
            break;
        case CharacterId:
        {
            const KHECore::KChar decodedChar = mCharCodec->decode( byte );
            content = decodedChar.isUndefined() ?
                i18nc( "character is not defined", "undef." ) :
                QString( (QChar)decodedChar );
            break;
        }
        default:
            ;
        }
        result = content;
    }
    else if( role == Qt::TextAlignmentRole )
        result = Qt::AlignRight;

    return result;
}

QVariant ByteTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
Q_UNUSED( orientation );

    QVariant result;

    if( role == Qt::DisplayRole )
    {
        QString titel =
            section == DecimalId ?     i18n("Decimal") :
            section == HexadecimalId ? i18n("Hexadecimal") :
            section == OctalId ?       i18n("Octal") :
            section == BinaryId ?      i18n("Binary") :
            section == CharacterId ?   i18n("Character") :
            QString();
        result = titel;
        return result;
    }
    else
        return QAbstractTableModel::headerData( section, orientation, role );
}

ByteTableModel::~ByteTableModel()
{
    delete mDecimalCodec;
    delete mHexadecimalCodec;
    delete mOctalCodec;
    delete mBinaryCodec;
    delete mCharCodec;
}

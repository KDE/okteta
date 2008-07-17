/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytetablemodel.h"

// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <valuecodec.h>
// KDE
#include <KLocale>


static const unsigned char ByteTableDefaultUndefinedChar = '?';
static const int ByteSetSize = 256;

ByteTableModel::ByteTableModel( QObject *parent )
 : QAbstractTableModel( parent ),
   mCharCodec( KHECore::KCharCodec::createCodec(KHECore::LocalEncoding) ),
   mUndefinedChar( ByteTableDefaultUndefinedChar )
{
    static const KHECore::KCoding CodingIds[NofOfValueCodings] =
    {
        KHECore::DecimalCoding,
        KHECore::HexadecimalCoding,
        KHECore::OctalCoding,
        KHECore::BinaryCoding
    };
    for( int i=0; i<NofOfValueCodings; ++i )
        mValueCodec[i] = KHECore::ValueCodec::createCodec( CodingIds[i] );
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
                i18nc( "@item:intable character is not defined", "undef." ) :
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
            section == DecimalId ?     i18nc("@title:column short for Decimal",    "Dec") :
            section == HexadecimalId ? i18nc("@title:column short for Hexadecimal","Hex") :
            section == OctalId ?       i18nc("@title:column short for Octal",      "Oct") :
            section == BinaryId ?      i18nc("@title:column short for Binary",     "Bin") :
            section == CharacterId ?   i18nc("@title:column short for Character",  "Char") :
            QString();
        result = titel;
    }
    else if( role == Qt::ToolTipRole )
    {
        const QString titel =
            section == DecimalId ?
                i18nc("@info:tooltip column contains the value in decimal format",    "Decimal") :
            section == HexadecimalId ?
                i18nc("@info:tooltip column contains the value in hexadecimal format","Hexadecimal") :
            section == OctalId ?
                i18nc("@info:tooltip column contains the value in octal format",      "Octal") :
            section == BinaryId ?
                i18nc("@info:tooltip column contains the value in binary format",     "Binary") :
            section == CharacterId ?
                i18nc("@info:tooltip column contains the character with the value",   "Character") :
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

#include "bytetablemodel.moc"

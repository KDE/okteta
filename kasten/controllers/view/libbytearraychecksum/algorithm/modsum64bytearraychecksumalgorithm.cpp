/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "modsum64bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QtEndian>


ModSum64ByteArrayChecksumAlgorithm::ModSum64ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
     i18nc("name of the checksum algorithm", "Modular sum 64-bit") )
{}

AbstractByteArrayChecksumParameterSet* ModSum64ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum64ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                            const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    const bool useLittleEndian = ( mParameterSet.endianness() == LittleEndian );
    quint64 modSum = useLittleEndian ?
        calculateModSumWithLittleEndian( model, range ) :
        calculateModSumWithBigEndian( model, range );

    modSum = ~modSum + 1;

    if( useLittleEndian )
        modSum = qbswap( modSum );

    *result = QString::fromLatin1("%1").arg( modSum, 16, 16, QChar::fromLatin1('0') );
    return true;
}

quint64 ModSum64ByteArrayChecksumAlgorithm::calculateModSumWithBigEndian( const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    quint64 modSum = 0x00000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        quint64 value = (quint64)( (quint8)(model->byte( i )) ) << 56;
        ++i;
        if( i<=range.end() )
        {
            value |= (quint64)( (quint8)(model->byte( i )) ) << 48;
            ++i;
            if( i<=range.end() )
            {
                value |= (quint64)( (quint8)(model->byte( i )) ) << 40;
                ++i;
                if( i<=range.end() )
                {
                    value |= (quint64)( (quint8)(model->byte( i )) ) << 32;
                    ++i;
                    if( i<=range.end() )
                    {
                        value |= (quint64)( (quint8)(model->byte( i )) ) << 24;
                        ++i;
                        if( i<=range.end() )
                        {
                            value |= (quint64)( (quint8)(model->byte( i )) ) << 16;
                            ++i;
                            if( i<=range.end() )
                            {
                                value |= (quint64)( (quint8)(model->byte( i )) ) << 8;
                                ++i;
                                if( i<=range.end() )
                                    value |= (quint64)( (quint8)(model->byte( i )) );
                            }
                        }
                    }
                }
            }
        }

        modSum += value;
#if 0
        const uchar value = (crcBits & 0xFF) + model->byte( i );
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }

    return modSum;
}

quint64 ModSum64ByteArrayChecksumAlgorithm::calculateModSumWithLittleEndian( const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    quint64 modSum = 0x00000000;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        quint64 value = (quint8)( model->byte(i) );
        ++i;
        if( i<=range.end() )
        {
            value |= (quint64)( (quint8)(model->byte( i )) ) << 8;
            ++i;
            if( i<=range.end() )
            {
                value |= (quint64)( (quint8)(model->byte( i )) ) << 16;
                ++i;
                if( i<=range.end() )
                {
                    value |= (quint64)( (quint8)(model->byte( i )) ) << 24;
                    ++i;
                    if( i<=range.end() )
                    {
                        value |= (quint64)( (quint8)(model->byte( i )) ) << 32;
                        ++i;
                        if( i<=range.end() )
                        {
                            value |= (quint64)( (quint8)(model->byte( i )) ) << 40;
                            ++i;
                            if( i<=range.end() )
                            {
                                value |= (quint64)( (quint8)(model->byte( i )) ) << 48;
                                ++i;
                                if( i<=range.end() )
                                    value |= (quint64)( (quint8)(model->byte( i )) ) << 56;
                            }
                        }
                    }
                }
            }
        }

        modSum += value;
#if 0
        const uchar value = (crcBits & 0xFF) + model->byte( i );
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }

    return modSum;
}

ModSum64ByteArrayChecksumAlgorithm::~ModSum64ByteArrayChecksumAlgorithm() {}

/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "modsum16bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


ModSum16ByteArrayChecksumAlgorithm::ModSum16ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
     i18nc("name of the checksum algorithm", "Modular sum 16-bit") )
{}

AbstractByteArrayChecksumParameterSet* ModSum16ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum16ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                            const KHECore::AbstractByteArrayModel* model, const KHE::Section& section ) const
{
    quint16 modSum = 0x0000;
    int nextBlockEnd = section.start() + CalculatedByteCountSignalLimit;

    // TODO: move padding checks into extra code before and after loop
    for( int i = section.start(); i<=section.end(); ++i )
    {
        quint16 value = (quint8)( model->datum(i) ) << 8;
        ++i;
        if( i<=section.end() )
            value += (quint8)( model->datum(i) );

        modSum += value;
#if 0
        const uchar value = (crcBits & 0xFF) + model->datum( i );
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( section.localIndex(i)+1 );
        }
    }

    *result = QString::number( modSum, 16 );
    return true;
}

ModSum16ByteArrayChecksumAlgorithm::~ModSum16ByteArrayChecksumAlgorithm() {}

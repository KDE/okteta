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

#include "modsum8bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtCore/QtGlobal>



ModSum8ByteArrayChecksumAlgorithm::ModSum8ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
     i18nc("name of the checksum algorithm", "Modular sum 8-bit") )
{}

AbstractByteArrayChecksumParameterSet* ModSum8ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool ModSum8ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                          const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    quint8 modSum = 0x00;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        modSum += (quint8)( model->byte(i) );
#if 0
        const uchar value = (crcBits & 0xFF) + model->datum( i );
        crcBits >>= 8;
        crcBits ^= lookupTable[value];
#endif
        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }
    modSum = ~modSum + 1;

    *result = QString::fromLatin1("%1").arg( modSum, 2, 16, QChar::fromLatin1('0') );
    return true;
}

ModSum8ByteArrayChecksumAlgorithm::~ModSum8ByteArrayChecksumAlgorithm() {}

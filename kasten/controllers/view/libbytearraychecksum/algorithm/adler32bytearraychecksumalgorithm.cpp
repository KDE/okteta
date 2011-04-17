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

#include "adler32bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>


static const int MOD_ADLER = 65521;


Adler32ByteArrayChecksumAlgorithm::Adler32ByteArrayChecksumAlgorithm()
  : AbstractByteArrayChecksumAlgorithm(
     i18nc("name of the checksum algorithm", "Adler-32") )
{}

AbstractByteArrayChecksumParameterSet* Adler32ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Adler32ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                           const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    quint32 a = 1;
    quint32 b = 0;

    // TODO: this is the "inefficient but straightforward implementation" from the Wikipedia entry, search for improved
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for( Okteta::Address i = range.start(); i<=range.end(); ++i )
    {
        a = (a + model->byte( i )) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;

        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }

    const quint32 sum = (b << 16) | a;
    *result = QString::fromLatin1("%1").arg( sum, 8, 16, QChar::fromLatin1('0') );

    return true;
}

Adler32ByteArrayChecksumAlgorithm::~Adler32ByteArrayChecksumAlgorithm() {}

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

// QCA
// need to have this first, as QCA needs QT_NO_CAST_FROM_ASCII disabled when included
#include <config-qca2.h> //krazy:excludeall=includes
#ifdef HAVE_QCA2
// disable QT_NO_CAST_FROM_ASCII
#ifdef QT_NO_CAST_FROM_ASCII
#undef QT_NO_CAST_FROM_ASCII
#endif
#include <QtCrypto>
#endif

#include "qca2bytearraychecksumalgorithm.h"

// Okteta core
#include <abstractbytearraymodel.h>
// Qt
#include <QtCore/QByteArray>


Qca2ByteArrayChecksumAlgorithm::Qca2ByteArrayChecksumAlgorithm( const QString& name, const QString& type )
  : AbstractByteArrayChecksumAlgorithm( name ),
    mType( type )
{}

AbstractByteArrayChecksumParameterSet* Qca2ByteArrayChecksumAlgorithm::parameterSet() { return &mParameterSet; }

bool Qca2ByteArrayChecksumAlgorithm::calculateChecksum( QString* result,
                                                        const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range ) const
{
    QCA::Hash hash( mType );

    // TODO: find a way without needing to copy, perhaps by smart iterator which can return spans of original data
    // TODO: see if buffer size could be a value which matches the algorithm and qca2

    char buffer[CalculatedByteCountSignalLimit];
    int bufferLength = CalculatedByteCountSignalLimit;
    Okteta::Address nextBlockEnd = range.start() + CalculatedByteCountSignalLimit;
    for( Okteta::Address i = range.start(); i<=range.end(); i+=CalculatedByteCountSignalLimit )
    {
        if( range.end() < i+CalculatedByteCountSignalLimit )
            bufferLength = range.end() - i + 1;
        model->copyTo( reinterpret_cast<Okteta::Byte*>(buffer), i, bufferLength );
        hash.update( buffer, bufferLength );

        if( i >= nextBlockEnd )
        {
            nextBlockEnd += CalculatedByteCountSignalLimit;
            emit calculatedBytes( range.localIndex(i)+1 );
        }
    }

    const QByteArray hashResult = hash.final().toByteArray();

    *result = QCA::arrayToHex( hashResult );
    return true;
}

Qca2ByteArrayChecksumAlgorithm::~Qca2ByteArrayChecksumAlgorithm() {}

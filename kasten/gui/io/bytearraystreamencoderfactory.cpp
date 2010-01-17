/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "bytearraystreamencoderfactory.h"

// lib
#include "bytearraytextstreamencoder.h"
#include "bytearrayvaluestreamencoder.h"
#include "bytearraysourcecodestreamencoder.h"
#include "bytearrayviewtextstreamencoder.h"
#include "bytearraybase64streamencoder.h"
#include "bytearraybase32streamencoder.h"
#include "bytearrayuuencodingstreamencoder.h"
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.h"
//// NEWBYTEARRAYSTREAMENCODER(end)
// Qt
#include <QtCore/QList>


namespace Kasten
{

QList<AbstractModelStreamEncoder*> ByteArrayStreamEncoderFactory::createStreamEncoders()
{
    QList<AbstractModelStreamEncoder*> result;

    result
        << new ByteArrayValueStreamEncoder()
        << new ByteArrayTextStreamEncoder()
        << new ByteArrayBase64StreamEncoder()
        << new ByteArrayBase32StreamEncoder()
        << new ByteArrayUuencodingStreamEncoder()
        << new ByteArraySourceCodeStreamEncoder()
        << new ByteArrayViewTextStreamEncoder();
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////     result
////         << new My_ByteArrayStreamEncoder();
//// NEWBYTEARRAYSTREAMENCODER(end)

    return result;
}

}

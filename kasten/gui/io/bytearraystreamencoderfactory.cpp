/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraystreamencoderfactory.hpp"

// lib
#include "streamencoder/chars/bytearraycharsstreamencoder.hpp"
#include "streamencoder/values/bytearrayvaluesstreamencoder.hpp"
#include "streamencoder/sourcecode/bytearraysourcecodestreamencoder.hpp"
#include "streamencoder/viewtext/bytearrayviewtextstreamencoder.hpp"
#include "streamencoder/base64/bytearraybase64streamencoder.hpp"
#include "streamencoder/base32/bytearraybase32streamencoder.hpp"
#include "streamencoder/base85/bytearraybase85streamencoder.hpp"
#include "streamencoder/uuencoding/bytearrayuuencodingstreamencoder.hpp"
#include "streamencoder/xxencoding/bytearrayxxencodingstreamencoder.hpp"
#include "streamencoder/srec/bytearraysrecstreamencoder.hpp"
#include "streamencoder/ihex/bytearrayihexstreamencoder.hpp"
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.hpp"
//// NEWBYTEARRAYSTREAMENCODER(end)
// Qt
#include <QVector>

namespace Kasten {

QVector<AbstractModelStreamEncoder*> ByteArrayStreamEncoderFactory::createStreamEncoders()
{
    const QVector<AbstractModelStreamEncoder*> result {
        new ByteArrayValuesStreamEncoder(),
        new ByteArrayCharsStreamEncoder(),
        new ByteArrayBase64StreamEncoder(),
        new ByteArrayBase32StreamEncoder(),
        new ByteArrayBase85StreamEncoder(),
        new ByteArrayUuencodingStreamEncoder(),
        new ByteArrayXxencodingStreamEncoder(),
        new ByteArrayIHexStreamEncoder(),
        new ByteArraySRecStreamEncoder(),
        new ByteArraySourceCodeStreamEncoder(),
        new ByteArrayViewTextStreamEncoder(),
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////         new My_ByteArrayStreamEncoder(),
//// NEWBYTEARRAYSTREAMENCODER(end)
    };
    return result;
}

}

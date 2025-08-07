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
#include "streamencoder/viewhtml/bytearrayviewhtmlstreamencoder.hpp"
#include "streamencoder/viewtext/bytearrayviewtextstreamencoder.hpp"
#include "streamencoder/base64/bytearraybase64streamencoder.hpp"
#include "streamencoder/base32/bytearraybase32streamencoder.hpp"
#include "streamencoder/base85/bytearraybase85streamencoder.hpp"
#include "streamencoder/uuencoding/bytearrayuuencodingstreamencoder.hpp"
#include "streamencoder/xxencoding/bytearrayxxencodingstreamencoder.hpp"
#include "streamencoder/srec/bytearraysrecstreamencoder.hpp"
#include "streamencoder/ihex/bytearrayihexstreamencoder.hpp"
#include "streamcompressor/bzip2/bytearraybzip2streamcompressor.hpp"
#include "streamcompressor/deflate/bytearraydeflatestreamcompressor.hpp"
#include "streamcompressor/gzip/bytearraygzipstreamcompressor.hpp"
#include "streamcompressor/xz/bytearrayxzstreamcompressor.hpp"
#include "streamcompressor/zstd/bytearrayzstdstreamcompressor.hpp"
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.hpp"
//// NEWBYTEARRAYSTREAMENCODER(end)

namespace Kasten {

namespace ByteArrayStreamEncoder {

template<typename T, typename ... Ptrs>
auto make_unique_vector(Ptrs&& ... ptrs)
{
    std::vector<std::unique_ptr<T>> vector;
    vector.reserve(sizeof...(Ptrs));
    ( vector.emplace_back(std::unique_ptr<T>(ptrs)), ... );
    return vector;
}

}

std::vector<std::unique_ptr<AbstractModelStreamEncoder>>
ByteArrayStreamEncoderFactory::createStreamEncoders()
{
    auto result = ByteArrayStreamEncoder::make_unique_vector<AbstractModelStreamEncoder>(
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
        new ByteArrayViewHtmlStreamEncoder(),
        new ByteArrayBZip2StreamCompressor(),
        new ByteArrayDeflateStreamCompressor(),
        new ByteArrayGZipStreamCompressor(),
        new ByteArrayXzStreamCompressor(),
        new ByteArrayZStdStreamCompressor()
//// NEWBYTEARRAYSTREAMENCODER(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////         new My_ByteArrayStreamEncoder(),
//// NEWBYTEARRAYSTREAMENCODER(end)
    );
    return result;
}

}

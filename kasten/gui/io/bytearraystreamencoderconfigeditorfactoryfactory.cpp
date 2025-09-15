/*
    SPDX-FileCopyrightText: 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraystreamencoderconfigeditorfactoryfactory.hpp"

// lib
#include "streamencoder/sourcecode//bytearraysourcecodestreamencoderconfigeditorfactory.hpp"
#include "streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/chars/bytearraycharsstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/base32/bytearraybase32streamencoderconfigeditorfactory.hpp"
#include "streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/xxencoding/bytearrayxxencodingstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/srec/bytearraysrecstreamencoderconfigeditorfactory.hpp"
#include "streamencoder/ihex/bytearrayihexstreamencoderconfigeditorfactory.hpp"
#include "streamcompressor/bzip2/bytearraybzip2streamcompressorconfigeditorfactory.hpp"
#include "streamcompressor/deflate/bytearraydeflatestreamcompressorconfigeditorfactory.hpp"
#include "streamcompressor/gzip/bytearraygzipstreamcompressorconfigeditorfactory.hpp"
#include "streamcompressor/xz/bytearrayxzstreamcompressorconfigeditorfactory.hpp"
#include "streamcompressor/zstd/bytearrayzstdstreamcompressorconfigeditorfactory.hpp"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the name of your header file of your streamencoder,
//// e.g.
//// #include "my_bytearraystreamencoder.hpp"
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)

namespace Kasten {

namespace ByteArrayStreamEncoderConfigEditorFactory {

template<typename T, typename ... Ptrs>
auto make_unique_vector(Ptrs&& ... ptrs)
{
    std::vector<std::unique_ptr<T>> vector;
    vector.reserve(sizeof...(Ptrs));
    ( vector.emplace_back(std::unique_ptr<T>(ptrs)), ... );
    return vector;
}

}

std::vector<std::unique_ptr<AbstractModelStreamEncoderConfigEditorFactory>> ByteArrayStreamEncoderConfigEditorFactoryFactory::createFactorys()
{
    auto result = ByteArrayStreamEncoderConfigEditorFactory::make_unique_vector<AbstractModelStreamEncoderConfigEditorFactory>(
        new ByteArraySourceCodeStreamEncoderConfigEditorFactory(),
        new ByteArrayValuesStreamEncoderConfigEditorFactory(),
        new ByteArrayCharsStreamEncoderConfigEditorFactory(),
        new ByteArrayBase32StreamEncoderConfigEditorFactory(),
        new ByteArraySRecStreamEncoderConfigEditorFactory(),
        new ByteArrayIHexStreamEncoderConfigEditorFactory(),
        new ByteArrayUuencodingStreamEncoderConfigEditorFactory(),
        new ByteArrayXxencodingStreamEncoderConfigEditorFactory(),
        new ByteArrayBZip2StreamCompressorConfigEditorFactory(),
        new ByteArrayDeflateStreamCompressorConfigEditorFactory(),
        new ByteArrayGZipStreamCompressorConfigEditorFactory(),
        new ByteArrayXzStreamCompressorConfigEditorFactory(),
        new ByteArrayZStdStreamCompressorConfigEditorFactory()
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(start)
//// Here add the creation of an object of your streamencoder class and add it to the list,
//// e.g.
////         new My_ByteArrayStreamEncoderConfigEditorFactory(),
//// NEWBYTEARRAYSTREAMENCODERCONFIGEDITORFACTORY(end)
    );
    return result;
}

}

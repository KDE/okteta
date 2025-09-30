/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayuuencodingstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearrayuuencodingstreamencoderconfigeditor.hpp"
#include "bytearrayuuencodingstreamencoder.hpp"

namespace Kasten {

ByteArrayUuencodingStreamEncoderConfigEditorFactory::ByteArrayUuencodingStreamEncoderConfigEditorFactory() = default;

ByteArrayUuencodingStreamEncoderConfigEditorFactory::~ByteArrayUuencodingStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayUuencodingStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const uuencodingStreamEncoder = qobject_cast<ByteArrayUuencodingStreamEncoder*>(encoder);

    if (uuencodingStreamEncoder) {
        result = std::make_unique<ByteArrayUuencodingStreamEncoderConfigEditor>(uuencodingStreamEncoder);
    }

    return result;
}

}

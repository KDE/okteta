/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxxencodingstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearrayxxencodingstreamencoderconfigeditor.hpp"
#include "bytearrayxxencodingstreamencoder.hpp"

namespace Kasten {

ByteArrayXxencodingStreamEncoderConfigEditorFactory::ByteArrayXxencodingStreamEncoderConfigEditorFactory() = default;

ByteArrayXxencodingStreamEncoderConfigEditorFactory::~ByteArrayXxencodingStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayXxencodingStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* xxencodingStreamEncoder = qobject_cast<ByteArrayXxencodingStreamEncoder*>(encoder);

    if (xxencodingStreamEncoder) {
        result = std::make_unique<ByteArrayXxencodingStreamEncoderConfigEditor>(xxencodingStreamEncoder);
    }

    return result;
}

}

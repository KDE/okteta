/*
    This file is part of the Kasten Framework, made within the KDE community.

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

AbstractModelStreamEncoderConfigEditor* ByteArrayUuencodingStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;
    auto* uuencodingStreamEncoder = qobject_cast<ByteArrayUuencodingStreamEncoder*>(encoder);

    if (uuencodingStreamEncoder) {
        result = new ByteArrayUuencodingStreamEncoderConfigEditor(uuencodingStreamEncoder);
    }

    return result;
}

}

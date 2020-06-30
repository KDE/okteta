/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybase32streamencoderconfigeditorfactory.hpp"

// lib
#include "bytearraybase32streamencoderconfigeditor.hpp"
#include "bytearraybase32streamencoder.hpp"

namespace Kasten {

ByteArrayBase32StreamEncoderConfigEditorFactory::ByteArrayBase32StreamEncoderConfigEditorFactory() = default;

ByteArrayBase32StreamEncoderConfigEditorFactory::~ByteArrayBase32StreamEncoderConfigEditorFactory() = default;

AbstractModelStreamEncoderConfigEditor* ByteArrayBase32StreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;
    auto* base32StreamEncoder = qobject_cast<ByteArrayBase32StreamEncoder*>(encoder);

    if (base32StreamEncoder) {
        result = new ByteArrayBase32StreamEncoderConfigEditor(base32StreamEncoder);
    }

    return result;
}

}

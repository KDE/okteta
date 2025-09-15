/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycharsstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearraycharsstreamencoderconfigeditor.hpp"
#include "bytearraycharsstreamencoder.hpp"

namespace Kasten {

ByteArrayCharsStreamEncoderConfigEditorFactory::ByteArrayCharsStreamEncoderConfigEditorFactory() = default;

ByteArrayCharsStreamEncoderConfigEditorFactory::~ByteArrayCharsStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayCharsStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* charsStreamEncoder = qobject_cast<ByteArrayCharsStreamEncoder*>(encoder);

    if (charsStreamEncoder) {
        result = std::make_unique<ByteArrayCharsStreamEncoderConfigEditor>(charsStreamEncoder);
    }

    return result;
}

}

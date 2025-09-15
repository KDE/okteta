/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluesstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearrayvaluesstreamencoderconfigeditor.hpp"
#include "bytearrayvaluesstreamencoder.hpp"

namespace Kasten {

ByteArrayValuesStreamEncoderConfigEditorFactory::ByteArrayValuesStreamEncoderConfigEditorFactory() = default;

ByteArrayValuesStreamEncoderConfigEditorFactory::~ByteArrayValuesStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayValuesStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* valuesStreamEncoder = qobject_cast<ByteArrayValuesStreamEncoder*>(encoder);

    if (valuesStreamEncoder) {
        result = std::make_unique<ByteArrayValuesStreamEncoderConfigEditor>(valuesStreamEncoder);
    }

    return result;
}

}

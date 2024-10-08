/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysourcecodestreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearraysourcecodestreamencoderconfigeditor.hpp"
#include "bytearraysourcecodestreamencoder.hpp"

namespace Kasten {

ByteArraySourceCodeStreamEncoderConfigEditorFactory::ByteArraySourceCodeStreamEncoderConfigEditorFactory() = default;

ByteArraySourceCodeStreamEncoderConfigEditorFactory::~ByteArraySourceCodeStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArraySourceCodeStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* sourceCodeStreamEncoder = qobject_cast<ByteArraySourceCodeStreamEncoder*>(encoder);

    if (sourceCodeStreamEncoder) {
        result = std::make_unique<ByteArraySourceCodeStreamEncoderConfigEditor>(sourceCodeStreamEncoder);
    }

    return result;
}

}

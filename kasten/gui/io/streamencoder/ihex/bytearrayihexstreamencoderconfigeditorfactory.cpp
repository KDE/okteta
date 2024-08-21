/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayihexstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearrayihexstreamencoderconfigeditor.hpp"
#include "bytearrayihexstreamencoder.hpp"

namespace Kasten {

ByteArrayIHexStreamEncoderConfigEditorFactory::ByteArrayIHexStreamEncoderConfigEditorFactory() = default;

ByteArrayIHexStreamEncoderConfigEditorFactory::~ByteArrayIHexStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArrayIHexStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* srecStreamEncoder = qobject_cast<ByteArrayIHexStreamEncoder*>(encoder);

    if (srecStreamEncoder) {
        result = std::make_unique<ByteArrayIHexStreamEncoderConfigEditor>(srecStreamEncoder);
    }

    return result;
}

}

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

AbstractModelStreamEncoderConfigEditor* ByteArrayIHexStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;
    auto* srecStreamEncoder = qobject_cast<ByteArrayIHexStreamEncoder*>(encoder);

    if (srecStreamEncoder) {
        result = new ByteArrayIHexStreamEncoderConfigEditor(srecStreamEncoder);
    }

    return result;
}

}

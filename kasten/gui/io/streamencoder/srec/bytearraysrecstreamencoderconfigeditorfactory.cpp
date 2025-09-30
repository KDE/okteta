/*
    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysrecstreamencoderconfigeditorfactory.hpp"

// lib
#include "bytearraysrecstreamencoderconfigeditor.hpp"
#include "bytearraysrecstreamencoder.hpp"

namespace Kasten {

ByteArraySRecStreamEncoderConfigEditorFactory::ByteArraySRecStreamEncoderConfigEditorFactory() = default;

ByteArraySRecStreamEncoderConfigEditorFactory::~ByteArraySRecStreamEncoderConfigEditorFactory() = default;

std::unique_ptr<AbstractModelStreamEncoderConfigEditor> ByteArraySRecStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    std::unique_ptr<AbstractModelStreamEncoderConfigEditor> result;
    auto* const srecStreamEncoder = qobject_cast<ByteArraySRecStreamEncoder*>(encoder);

    if (srecStreamEncoder) {
        result = std::make_unique<ByteArraySRecStreamEncoderConfigEditor>(srecStreamEncoder);
    }

    return result;
}

}

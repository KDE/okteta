/*
    This file is part of the Kasten Framework, made within the KDE community.

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

AbstractModelStreamEncoderConfigEditor* ByteArraySRecStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;
    auto* srecStreamEncoder = qobject_cast<ByteArraySRecStreamEncoder*>(encoder);

    if (srecStreamEncoder) {
        result = new ByteArraySRecStreamEncoderConfigEditor(srecStreamEncoder);
    }

    return result;
}

}

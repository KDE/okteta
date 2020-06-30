/*
    This file is part of the Kasten Framework, made within the KDE community.

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

AbstractModelStreamEncoderConfigEditor* ByteArrayValuesStreamEncoderConfigEditorFactory::tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const
{
    AbstractModelStreamEncoderConfigEditor* result = nullptr;
    auto* valuesStreamEncoder = qobject_cast<ByteArrayValuesStreamEncoder*>(encoder);

    if (valuesStreamEncoder) {
        result = new ByteArrayValuesStreamEncoderConfigEditor(valuesStreamEncoder);
    }

    return result;
}

}

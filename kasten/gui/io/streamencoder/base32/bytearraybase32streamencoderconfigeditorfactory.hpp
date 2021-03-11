/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITORFACTORY_HPP
#define KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITORFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditorFactory>

namespace Kasten {

class ByteArrayBase32StreamEncoderConfigEditorFactory : public AbstractModelStreamEncoderConfigEditorFactory
{
public:
    ByteArrayBase32StreamEncoderConfigEditorFactory();
    ~ByteArrayBase32StreamEncoderConfigEditorFactory() override;

public:
    AbstractModelStreamEncoderConfigEditor* tryCreateConfigEditor(AbstractModelStreamEncoder* encoder) const override;
};

}

#endif

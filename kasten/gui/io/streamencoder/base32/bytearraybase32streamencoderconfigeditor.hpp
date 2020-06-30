/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYBASE32STREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearraybase32streamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;

namespace Kasten {

class ByteArrayBase32StreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayBase32StreamEncoderConfigEditor(ByteArrayBase32StreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayBase32StreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayBase32StreamEncoder* mEncoder;
    Base32StreamEncoderSettings mSettings;

    KComboBox* mEncodingSelect;
};

}

#endif

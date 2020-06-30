/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYSRECSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYSRECSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearraysrecstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;

namespace Kasten {

class ByteArraySRecStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArraySRecStreamEncoderConfigEditor(ByteArraySRecStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArraySRecStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    AbstractSelectionView* createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArraySRecStreamEncoder* mEncoder;
    SRecStreamEncoderSettings mSettings;

    KComboBox* mAddressSizeSelect;
};

}

#endif

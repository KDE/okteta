/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYIHEXSTREAMENCODERCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYIHEXSTREAMENCODERCONFIGEDITOR_HPP

// lib
#include "bytearrayihexstreamencoder.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;

namespace Kasten {

class ByteArrayIHexStreamEncoderConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayIHexStreamEncoderConfigEditor(ByteArrayIHexStreamEncoder* encoder, QWidget* parent = nullptr);
    ~ByteArrayIHexStreamEncoderConfigEditor() override;

public: // AbstractModelStreamEncoderConfigEditor API
    [[nodiscard]]
    std::unique_ptr<AbstractSelectionView> createPreviewView() const override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayIHexStreamEncoder* const mEncoder;

    IHexStreamEncoderSettings mSettings;

    KComboBox* mAddressSizeSelect;
};

}

#endif

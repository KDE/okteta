/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYXZSTREAMCOMPRESSORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYXZSTREAMCOMPRESSORCONFIGEDITOR_HPP

// lib
#include "bytearrayxzstreamcompressor.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QCheckBox;
class QSpinBox;

namespace Kasten {

class ByteArrayXzStreamCompressorConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayXzStreamCompressorConfigEditor(ByteArrayXzStreamCompressor* encoder, QWidget* parent = nullptr);
    ~ByteArrayXzStreamCompressorConfigEditor() override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayXzStreamCompressor* const m_encoder;

    XzStreamCompressorSettings m_settings;

    QSpinBox* m_compressionPresetLevelSelect;
    QCheckBox* m_useExtremePresetLevelVariantCheck;
    KComboBox* m_integrityCheckSelect;
};

}

#endif

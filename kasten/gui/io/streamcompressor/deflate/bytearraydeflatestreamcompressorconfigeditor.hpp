/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYDEFLATESTREAMCOMPRESSORCONFIGEDITOR_HPP

// lib
#include "bytearraydeflatestreamcompressor.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QCheckBox;
class QSpinBox;

namespace Kasten {

class ByteArrayDeflateStreamCompressorConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayDeflateStreamCompressorConfigEditor(ByteArrayDeflateStreamCompressor* encoder, QWidget* parent = nullptr);
    ~ByteArrayDeflateStreamCompressorConfigEditor() override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayDeflateStreamCompressor* const m_encoder;

    DeflateStreamCompressorSettings m_settings;

    QSpinBox* m_compressionLevelSelect;
    KComboBox* m_strategySelect;
    QCheckBox* m_createRawCheck;
};

}

#endif

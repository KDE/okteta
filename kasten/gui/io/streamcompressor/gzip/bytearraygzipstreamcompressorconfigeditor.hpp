/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYGZIPSTREAMCOMPRESSORCONFIGEDITOR_HPP

// lib
#include "bytearraygzipstreamcompressor.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class KComboBox;
class QLineEdit;
class QSpinBox;

namespace Kasten {

class ByteArrayGZipStreamCompressorConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayGZipStreamCompressorConfigEditor(ByteArrayGZipStreamCompressor* encoder, QWidget* parent = nullptr);
    ~ByteArrayGZipStreamCompressorConfigEditor() override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayGZipStreamCompressor* const m_encoder;

    GZipStreamCompressorSettings m_settings;

    QSpinBox* m_compressionLevelSelect;
    KComboBox* m_strategySelect;
    QLineEdit* m_fileNameEdit;
    QLineEdit* m_commentEdit;
};

}

#endif

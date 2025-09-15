/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYZSTDSTREAMCOMPRESSORCONFIGEDITOR_HPP

// lib
#include "bytearrayzstdstreamcompressor.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class QCheckBox;
class QSpinBox;

namespace Kasten {

class ByteArrayZStdStreamCompressorConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayZStdStreamCompressorConfigEditor(ByteArrayZStdStreamCompressor* encoder, QWidget* parent = nullptr);
    ~ByteArrayZStdStreamCompressorConfigEditor() override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayZStdStreamCompressor* const m_encoder;

    ZStdStreamCompressorSettings m_settings;

    QSpinBox* m_compressionLevelSelect;
    QCheckBox* m_writeFrameContentSizeSelect;
    QCheckBox* m_writeFrameChecksumSelect;
};

}

#endif

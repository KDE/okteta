/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSORCONFIGEDITOR_HPP
#define KASTEN_BYTEARRAYBZIP2STREAMCOMPRESSORCONFIGEDITOR_HPP

// lib
#include "bytearraybzip2streamcompressor.hpp"
// Kasten gui
#include <Kasten/AbstractModelStreamEncoderConfigEditor>

class QSlider;

namespace Kasten {

class ByteArrayBZip2StreamCompressorConfigEditor : public AbstractModelStreamEncoderConfigEditor
{
    Q_OBJECT

public:
    explicit ByteArrayBZip2StreamCompressorConfigEditor(ByteArrayBZip2StreamCompressor* encoder, QWidget* parent = nullptr);
    ~ByteArrayBZip2StreamCompressorConfigEditor() override;

private Q_SLOTS:
    void onSettingsChanged();

private:
    ByteArrayBZip2StreamCompressor* const m_encoder;

    BZip2StreamCompressorSettings m_settings;

    QSlider* m_blockSizeSelect;
};

}

#endif

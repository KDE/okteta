/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayzstdstreamcompressorconfigeditor.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QCheckBox>
#include <QSpinBox>

namespace Kasten {

ByteArrayZStdStreamCompressorConfigEditor::ByteArrayZStdStreamCompressorConfigEditor(ByteArrayZStdStreamCompressor* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , m_encoder(encoder)
{
    m_settings = m_encoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // compression level
    const QString compressionLevelLabel = i18nc("@label:spinbox", "Compression level:");

    m_compressionLevelSelect = new QSpinBox(this);
    m_compressionLevelSelect->setRange(0, 9);
    m_compressionLevelSelect->setSpecialValueText(i18nc("@item:inmenu compression level","Default"));
    m_compressionLevelSelect->setValue(m_settings.compressionLevel);
    connect(m_compressionLevelSelect, qOverload<int>(&QSpinBox::valueChanged),
            this, &ByteArrayZStdStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(compressionLevelLabel, m_compressionLevelSelect);

    // write frame content size option
    const QString writeFrameContentSizeLabel = i18nc("@option:check", "Content size in frame header:");

    m_writeFrameContentSizeSelect = new QCheckBox(this);
    m_writeFrameContentSizeSelect->setChecked(m_settings.writeFrameContentSize);
    connect(m_writeFrameContentSizeSelect, &QCheckBox::toggled, this, &ByteArrayZStdStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(writeFrameContentSizeLabel, m_writeFrameContentSizeSelect);

    // write frame content size option
    const QString writeFrameChecksumLabel = i18nc("@option:check", "Content checksum at frame end:");

    m_writeFrameChecksumSelect = new QCheckBox(this);
    m_writeFrameChecksumSelect->setChecked(m_settings.writeFrameChecksum);
    connect(m_writeFrameChecksumSelect, &QCheckBox::toggled, this, &ByteArrayZStdStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(writeFrameChecksumLabel, m_writeFrameChecksumSelect);
}

ByteArrayZStdStreamCompressorConfigEditor::~ByteArrayZStdStreamCompressorConfigEditor() = default;

void ByteArrayZStdStreamCompressorConfigEditor::onSettingsChanged()
{
    m_settings.compressionLevel = m_compressionLevelSelect->value();
    m_settings.writeFrameContentSize = m_writeFrameContentSizeSelect->isChecked();
    m_settings.writeFrameChecksum = m_writeFrameChecksumSelect->isChecked();

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearrayzstdstreamcompressorconfigeditor.cpp"

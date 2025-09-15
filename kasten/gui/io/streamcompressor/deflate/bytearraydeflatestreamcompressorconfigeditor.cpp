/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraydeflatestreamcompressorconfigeditor.hpp"

// KF
#include <KComboBox>
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QCheckBox>
#include <QSpinBox>

namespace Kasten {

ByteArrayDeflateStreamCompressorConfigEditor::ByteArrayDeflateStreamCompressorConfigEditor(ByteArrayDeflateStreamCompressor* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , m_encoder(encoder)
{
    m_settings = m_encoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // compression level
    const QString compressionLevelLabel = i18nc("@label:spinbox", "Compression level:");

    m_compressionLevelSelect = new QSpinBox(this);
    m_compressionLevelSelect->setRange(-1, 9);
    m_compressionLevelSelect->setSpecialValueText(i18nc("@item:inmenu compression level","Default"));
    m_compressionLevelSelect->setValue(m_settings.compressionLevel);
    connect(m_compressionLevelSelect, qOverload<int>(&QSpinBox::valueChanged),
            this, &ByteArrayDeflateStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(compressionLevelLabel, m_compressionLevelSelect);

    // compression strategy
    const QString strategyLabel =
        i18nc("@label:listbox", "Compression strategy:");

    m_strategySelect = new KComboBox(this);
    const QStringList list {
        i18nc("@item:inmenu compression strategy",
              "Default"),
        i18nc("@item:inmenu compression strategy",
              "Filter"),
        i18nc("@item:inmenu compression strategy",
              "Huffman Encoding Only"),
        i18nc("@item:inmenu compression strategy",
              "Run-Length Encoding"),
        i18nc("@item:inmenu compression strategy",
              "Fixed Huffman Encoding"),
    };
    m_strategySelect->addItems(list);
    m_strategySelect->setCurrentIndex(static_cast<int>(m_settings.strategyId));
    connect(m_strategySelect, qOverload<int>(&KComboBox::activated),
            this, &ByteArrayDeflateStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(strategyLabel, m_strategySelect);

    // create raw option
    const QString createRawLabel =
        i18nc("@option:check Create deflate stream without headers or footers",
              "Create raw variant:");

    m_createRawCheck = new QCheckBox(this);
    const bool createRaw = (m_settings.formatId == DeflateStreamCompressorSettings::FormatId::Raw);
    m_createRawCheck->setChecked(createRaw);
    connect(m_createRawCheck, &QCheckBox::toggled, this, &ByteArrayDeflateStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(createRawLabel, m_createRawCheck);
}

ByteArrayDeflateStreamCompressorConfigEditor::~ByteArrayDeflateStreamCompressorConfigEditor() = default;

void ByteArrayDeflateStreamCompressorConfigEditor::onSettingsChanged()
{
    m_settings.compressionLevel = m_compressionLevelSelect->value();
    m_settings.strategyId = static_cast<DeflateStreamCompressorSettings::StrategyId>(m_strategySelect->currentIndex());
    m_settings.formatId = m_createRawCheck->isChecked() ? DeflateStreamCompressorSettings::FormatId::Raw : DeflateStreamCompressorSettings::FormatId::Full;

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearraydeflatestreamcompressorconfigeditor.cpp"

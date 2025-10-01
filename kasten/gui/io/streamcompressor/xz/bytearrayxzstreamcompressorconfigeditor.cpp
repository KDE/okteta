/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxzstreamcompressorconfigeditor.hpp"

// KF
#include <KComboBox>
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QCheckBox>
#include <QSpinBox>

namespace Kasten {

ByteArrayXzStreamCompressorConfigEditor::ByteArrayXzStreamCompressorConfigEditor(ByteArrayXzStreamCompressor* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , m_encoder(encoder)
{
    m_settings = m_encoder->settings();

    auto* const pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // compression level
    const QString compressionPresetLevelLabel = i18nc("@label:spinbox", "Compression preset level:");

    m_compressionPresetLevelSelect = new QSpinBox(this);
    m_compressionPresetLevelSelect->setRange(-1, 9);
    m_compressionPresetLevelSelect->setSpecialValueText(i18nc("@item:inmenu compression preset level","Default"));
    m_compressionPresetLevelSelect->setValue(m_settings.compressionPresetLevel);
    connect(m_compressionPresetLevelSelect, qOverload<int>(&QSpinBox::valueChanged),
            this, &ByteArrayXzStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(compressionPresetLevelLabel, m_compressionPresetLevelSelect);

    // use extreme variant
    const QString m_useExtremePresetLevelVariantLabel =
        i18nc("@option:check Use extreme variant of compression preset level",
              "Use extreme variant:");

    m_useExtremePresetLevelVariantCheck = new QCheckBox(this);
    const bool useExtreme = (m_settings.compressionPresetLevelVariantId == XzStreamCompressorSettings::CompressionPresetLevelVariantId::Extreme);
    m_useExtremePresetLevelVariantCheck->setChecked(useExtreme);
    connect(m_useExtremePresetLevelVariantCheck, &QCheckBox::toggled, this, &ByteArrayXzStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(m_useExtremePresetLevelVariantLabel, m_useExtremePresetLevelVariantCheck);

    // integrity check
    const QString strategyLabel =
        i18nc("@label:listbox", "Integrity check:");

    m_integrityCheckSelect = new KComboBox(this);
    const QStringList list {
        i18nc("@item:inmenu integrity check",
              "None"),
        i18nc("@item:inmenu integrity check",
              "CRC32"),
        i18nc("@item:inmenu integrity check",
              "CRC64"),
        i18nc("@item:inmenu integrity check",
              "SHA-256"),
    };
    m_integrityCheckSelect->addItems(list);
    m_integrityCheckSelect->setCurrentIndex(static_cast<int>(m_settings.integrityCheckId));
    connect(m_integrityCheckSelect, qOverload<int>(&KComboBox::activated),
            this, &ByteArrayXzStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(strategyLabel, m_integrityCheckSelect);
}

ByteArrayXzStreamCompressorConfigEditor::~ByteArrayXzStreamCompressorConfigEditor() = default;

void ByteArrayXzStreamCompressorConfigEditor::onSettingsChanged()
{
    m_settings.compressionPresetLevel = m_compressionPresetLevelSelect->value();
    m_settings.compressionPresetLevelVariantId = m_useExtremePresetLevelVariantCheck->isChecked() ? XzStreamCompressorSettings::CompressionPresetLevelVariantId::Extreme : XzStreamCompressorSettings::CompressionPresetLevelVariantId::Normal;
    m_settings.integrityCheckId = static_cast<XzStreamCompressorSettings::IntegrityCheckId>(m_integrityCheckSelect->currentIndex());

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearrayxzstreamcompressorconfigeditor.cpp"

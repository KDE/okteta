/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraygzipstreamcompressorconfigeditor.hpp"

// lib
#include "iso8859_1stringvalidator.hpp"
// KF
#include <KComboBox>
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

namespace Kasten {

ByteArrayGZipStreamCompressorConfigEditor::ByteArrayGZipStreamCompressorConfigEditor(ByteArrayGZipStreamCompressor* encoder, QWidget* parent)
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
            this, &ByteArrayGZipStreamCompressorConfigEditor::onSettingsChanged);
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
            this, &ByteArrayGZipStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(strategyLabel, m_strategySelect);

    // file name
    const QString fileNameLabel = i18nc("@label:textbox", "File name:");

    auto* iso8859_1StringValidator = new Okteta::Iso8859_1StringValidator(this);
    m_fileNameEdit = new QLineEdit(this);
    m_fileNameEdit->setClearButtonEnabled(true);
    m_fileNameEdit->setText(m_settings.fileName);
    m_fileNameEdit->setValidator(iso8859_1StringValidator);
    connect(m_fileNameEdit, &QLineEdit::textChanged,
            this, &ByteArrayGZipStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(fileNameLabel, m_fileNameEdit);

    // comment
    const QString commentLabel = i18nc("@label:textbox", "Comment:");

    // TODO: should be QTextEdit, needs custom solution to limit encoding and \r line breaks
    m_commentEdit = new QLineEdit(this);
    m_commentEdit->setClearButtonEnabled(true);
    m_commentEdit->setText(m_settings.comment);
    m_commentEdit->setValidator(iso8859_1StringValidator);
    connect(m_commentEdit, &QLineEdit::textChanged,
            this, &ByteArrayGZipStreamCompressorConfigEditor::onSettingsChanged);
    pageLayout->addRow(commentLabel, m_commentEdit);
}

ByteArrayGZipStreamCompressorConfigEditor::~ByteArrayGZipStreamCompressorConfigEditor() = default;

void ByteArrayGZipStreamCompressorConfigEditor::onSettingsChanged()
{
    m_settings.compressionLevel = m_compressionLevelSelect->value();
    m_settings.strategyId = static_cast<GZipStreamCompressorSettings::StrategyId>(m_strategySelect->currentIndex());
    m_settings.fileName = m_fileNameEdit->text();
    m_settings.comment = m_commentEdit->text();

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearraygzipstreamcompressorconfigeditor.cpp"

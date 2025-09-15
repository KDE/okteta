/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycharsstreamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>
#include <QLabel>

namespace Kasten {

ByteArrayCharsStreamEncoderConfigEditor::ByteArrayCharsStreamEncoderConfigEditor(ByteArrayCharsStreamEncoder* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , m_encoder(encoder)
{
    m_settings = m_encoder->settings();

    auto* pageLayout = new QFormLayout(this);   // unknown rows
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // m_escapeNonprintableSelect mode
    const QString encodeModeLabel =
        i18nc("@label:listbox",
              "Non-printable encode mode:");

    const QStringList encodeModeList {
        i18nc("@item:inmenu encode mode",
              "Substitute"),
        i18nc("@item:inmenu encode mode",
              "Escape C-Style"),
    };

    m_encodeModeSelect = new KComboBox(this);
    m_encodeModeSelect->addItems(encodeModeList);
    m_encodeModeSelect->setCurrentIndex(static_cast<int>(m_settings.encodeMode));
    connect(m_encodeModeSelect, qOverload<int>(&KComboBox::activated),
            this, &ByteArrayCharsStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(encodeModeLabel, m_encodeModeSelect);

    // nonprintable chars escape mode
    m_escapedValueCodingLabel = new QLabel(this);
    m_escapedValueCodingLabel->setText(i18nc("@label:listbox",
                                             "Encoding for escaped chars:"));

    const QStringList escapedValueCodingList {
        i18nc("@item:inmenu escape value encoding",
              "Hexadecimal"),
        i18nc("@item:inmenu escape value encoding",
              "Octal"),
    };

    m_escapedValueCodingSelect = new KComboBox(this);
    m_escapedValueCodingSelect->addItems(escapedValueCodingList);
    m_escapedValueCodingSelect->setCurrentIndex(static_cast<int>(m_settings.escapedValueCoding));
    m_escapedValueCodingSelect->setEnabled(m_encodeModeSelect->currentIndex());
    connect(m_escapedValueCodingSelect, qOverload<int>(&KComboBox::activated),
            this, &ByteArrayCharsStreamEncoderConfigEditor::onSettingsChanged);
    connect(m_encodeModeSelect, qOverload<int>(&KComboBox::activated),
            this, &ByteArrayCharsStreamEncoderConfigEditor::setCodingSelectEnabled);
    setCodingSelectEnabled(m_encodeModeSelect->currentIndex());
    pageLayout->addRow(m_escapedValueCodingLabel, m_escapedValueCodingSelect);
}

ByteArrayCharsStreamEncoderConfigEditor::~ByteArrayCharsStreamEncoderConfigEditor() = default;

std::unique_ptr<AbstractSelectionView> ByteArrayCharsStreamEncoderConfigEditor::createPreviewView() const
{
    return std::make_unique<ByteArrayTextStreamEncoderPreview>(m_encoder);
}

void ByteArrayCharsStreamEncoderConfigEditor::setCodingSelectEnabled(int encodeModeIndex)
{
    const bool isEscapeStyle =
        (CharsStreamEncoderSettings::EncodeMode::EscapeCStyle == static_cast<CharsStreamEncoderSettings::EncodeMode>(encodeModeIndex));
    m_escapedValueCodingLabel->setEnabled(isEscapeStyle);
    m_escapedValueCodingSelect->setEnabled(isEscapeStyle);
};

void ByteArrayCharsStreamEncoderConfigEditor::onSettingsChanged()
{
    m_settings.encodeMode = static_cast<CharsStreamEncoderSettings::EncodeMode>(m_encodeModeSelect->currentIndex());
    m_settings.escapedValueCoding = static_cast<CharsStreamEncoderSettings::EscapedValueCoding>(m_escapedValueCodingSelect->currentIndex());

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearraycharsstreamencoderconfigeditor.cpp"

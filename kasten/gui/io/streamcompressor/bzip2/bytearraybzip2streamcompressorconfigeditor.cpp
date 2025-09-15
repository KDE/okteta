/*
    SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybzip2streamcompressorconfigeditor.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

namespace Kasten {

ByteArrayBZip2StreamCompressorConfigEditor::ByteArrayBZip2StreamCompressorConfigEditor(ByteArrayBZip2StreamCompressor* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , m_encoder(encoder)
{
    m_settings = m_encoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // block size
    const QString blockSizeLabel = i18nc("@label:spinbox bzip2 ", "Block size:");

    auto* blockSizeSelectLayout = new QHBoxLayout;
    blockSizeSelectLayout->setContentsMargins(0, 0, 0, 0);
    m_blockSizeSelect = new QSlider(this);
    m_blockSizeSelect->setOrientation(Qt::Horizontal);
    m_blockSizeSelect->setRange(1, 9);
    m_blockSizeSelect->setPageStep(1);
    m_blockSizeSelect->setTickPosition(QSlider::TicksBelow);
    m_blockSizeSelect->setTickInterval(1);
    m_blockSizeSelect->setValue(m_settings.blockSize);
    connect(m_blockSizeSelect, &QSlider::valueChanged,
            this, &ByteArrayBZip2StreamCompressorConfigEditor::onSettingsChanged);
    blockSizeSelectLayout->addWidget(m_blockSizeSelect);

    auto* blockSizeValueLabel = new QLabel(this);
    blockSizeValueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    auto updateValueLabel = [blockSizeValueLabel](int value) {
        blockSizeValueLabel->setText(i18n("%1 kB", value * 100));
    };
    updateValueLabel(m_blockSizeSelect->value());
    connect(m_blockSizeSelect, &QSlider::valueChanged, blockSizeValueLabel, updateValueLabel);
    blockSizeSelectLayout->addWidget(blockSizeValueLabel);

    pageLayout->addRow(blockSizeLabel, blockSizeSelectLayout);
}

ByteArrayBZip2StreamCompressorConfigEditor::~ByteArrayBZip2StreamCompressorConfigEditor() = default;

void ByteArrayBZip2StreamCompressorConfigEditor::onSettingsChanged()
{
    m_settings.blockSize = m_blockSizeSelect->value();

    m_encoder->setSettings(m_settings);
}

}

#include "moc_bytearraybzip2streamcompressorconfigeditor.cpp"

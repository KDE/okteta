/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrandomdatageneratorconfigeditor.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QSpinBox>
#include <QFormLayout>

namespace Kasten {

ByteArrayRandomDataGeneratorConfigEditor::ByteArrayRandomDataGeneratorConfigEditor(ByteArrayRandomDataGenerator* generator, QWidget* parent)
    : AbstractModelDataGeneratorConfigEditor(parent)
    , mGenerator(generator)
{
    mSettings = mGenerator->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // number
    const QString numberInputLabel =
        i18nc("@label:spinbox size of the bytearray to generate",
              "&Size (bytes):");
    mSizeInput = new QSpinBox(this);
    mSizeInput->setRange(1, INT_MAX);
    mSizeInput->setValue(mSettings.size);
    connect(mSizeInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ByteArrayRandomDataGeneratorConfigEditor::onSettingsChanged);
    const QString numberWhatsThis =
        i18nc("@info:whatsthis",
              "Enter the size of the bytearray to generate.");
    mSizeInput->setWhatsThis(numberWhatsThis);

    pageLayout->addRow(numberInputLabel, mSizeInput);
}

ByteArrayRandomDataGeneratorConfigEditor::~ByteArrayRandomDataGeneratorConfigEditor() = default;

QString ByteArrayRandomDataGeneratorConfigEditor::name() const
{
    return i18nc("@item name of the generated data", "Random Data");
}

// TODO: get char codec
#if 0
void InsertRandomDataDialog::setCharCodec(const QString& codecName)
{
    mRandomDataEdit->setCharCodec(codecName);
}
#endif

void ByteArrayRandomDataGeneratorConfigEditor::onSettingsChanged()
{
    mSettings.size = mSizeInput->value();

    mGenerator->setSettings(mSettings);
}

}

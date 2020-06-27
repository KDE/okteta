/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraypatterngeneratorconfigeditor.hpp"

// lib
#include <bytearraycombobox.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QSpinBox>
#include <QFormLayout>

namespace Kasten {

ByteArrayPatternGeneratorConfigEditor::ByteArrayPatternGeneratorConfigEditor(ByteArrayPatternGenerator* generator, QWidget* parent)
    : AbstractModelDataGeneratorConfigEditor(parent)
    , mGenerator(generator)
{
    mSettings = mGenerator->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // pattern
    const QString patternEditLabel =
        i18nc("@label:textbox",
              "Pattern:");
    mPatternEdit = new Okteta::ByteArrayComboBox(this);
    mPatternEdit->setByteArray(mSettings.pattern);
    connect(mPatternEdit, &Okteta::ByteArrayComboBox::byteArrayChanged, this, &ByteArrayPatternGeneratorConfigEditor::onSettingsChanged);
    connect(mPatternEdit, &Okteta::ByteArrayComboBox::byteArrayChanged, this, &ByteArrayPatternGeneratorConfigEditor::onPatternChanged);
    const QString inputWhatsThis =
        i18nc("@info:whatsthis",
              "Enter a pattern to search for, or select a previous pattern from the list.");
    mPatternEdit->setWhatsThis(inputWhatsThis);

    pageLayout->addRow(patternEditLabel, mPatternEdit);

    // number
    const QString numberInputLabel =
        i18nc("@label:spinbox number of times to insert the pattern",
              "&Number:");
    mNumberInput = new QSpinBox(this);
    mNumberInput->setRange(1, INT_MAX);
    mNumberInput->setValue(mSettings.count);
    connect(mNumberInput, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ByteArrayPatternGeneratorConfigEditor::onSettingsChanged);
    const QString numberWhatsThis =
        i18nc("@info:whatsthis",
              "Enter the number of times the pattern should be inserted.");
    mNumberInput->setWhatsThis(numberWhatsThis);

    pageLayout->addRow(numberInputLabel, mNumberInput);
}

ByteArrayPatternGeneratorConfigEditor::~ByteArrayPatternGeneratorConfigEditor() = default;

bool ByteArrayPatternGeneratorConfigEditor::isValid() const { return (!mSettings.pattern.isEmpty()); }

QString ByteArrayPatternGeneratorConfigEditor::name() const
{
    return i18nc("@item name of the generated data", "Pattern");
}

// TODO: get char codec
#if 0
void InsertPatternDialog::setCharCodec(const QString& codecName)
{
    mPatternEdit->setCharCodec(codecName);
}
#endif

void ByteArrayPatternGeneratorConfigEditor::onSettingsChanged()
{
    mSettings.pattern = mPatternEdit->byteArray();
    mSettings.count = mNumberInput->value();

    mGenerator->setSettings(mSettings);
}

void ByteArrayPatternGeneratorConfigEditor::onPatternChanged(const QByteArray& pattern)
{
    emit validityChanged(!pattern.isEmpty());
}

}

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraybase32streamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>
#include <QLabel>

namespace Kasten {

ByteArrayBase32StreamEncoderConfigEditor::ByteArrayBase32StreamEncoderConfigEditor(ByteArrayBase32StreamEncoder* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , mEncoder(encoder)
{
    mSettings = mEncoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // data type
    const QString encodingTypeLabel =
        i18nc("@label:listbox the type of the used encoding: Classic, Base32hex or z-base-32.",
              "Encoding:");

    mEncodingSelect = new KComboBox(this);
    const QStringList list {
        i18nc("@item:inmenu Doing the base32 using the classical encoding",
              "Classic"),
        i18nc("@item:inmenu Doing the base32 using the Base32hex encoding",
              "Base32hex"),
        i18nc("@item:inmenu Doing the base32 using the z-base-32 encoding",
              "z-base-32"),
    };
    mEncodingSelect->addItems(list);
    mEncodingSelect->setCurrentIndex(static_cast<int>(mSettings.algorithmId));
    connect(mEncodingSelect, QOverload<int>::of(&KComboBox::activated),
            this, &ByteArrayBase32StreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(encodingTypeLabel, mEncodingSelect);
}

ByteArrayBase32StreamEncoderConfigEditor::~ByteArrayBase32StreamEncoderConfigEditor() = default;

AbstractSelectionView* ByteArrayBase32StreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview(mEncoder);
}

void ByteArrayBase32StreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.algorithmId = static_cast<Base32StreamEncoderSettings::AlgorithmId>(mEncodingSelect->currentIndex());

    mEncoder->setSettings(mSettings);
}

}

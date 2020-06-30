/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayuuencodingstreamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

namespace Kasten {

ByteArrayUuencodingStreamEncoderConfigEditor::ByteArrayUuencodingStreamEncoderConfigEditor(ByteArrayUuencodingStreamEncoder* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , mEncoder(encoder)
{
    mSettings = mEncoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // internal file name
    const QString fileNameLabel =
        i18nc("@label:textbox file name internally given to the encoded data",
              "Internal name of file:");

    mFileNameEdit = new QLineEdit(this);
    mFileNameEdit->setClearButtonEnabled(true);
    mFileNameEdit->setText(mSettings.fileName);
    connect(mFileNameEdit, &QLineEdit::textChanged, this, &ByteArrayUuencodingStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(fileNameLabel, mFileNameEdit);

    // data type
    const QString encodingTypeLabel =
        i18nc("@label:listbox the type of the used encoding: historical or Base64.",
              "Encoding:");

    mEncodingSelect = new KComboBox(this);
    const QStringList list {
        i18nc("@item:inmenu Doing the uuencoding using the historical encoding",
              "Historical"),
        i18nc("@item:inmenu Doing the uuencoding using the base64 encoding",
              "Base64"),
    };
    mEncodingSelect->addItems(list);
    mEncodingSelect->setCurrentIndex(static_cast<int>(mSettings.algorithmId));
    connect(mEncodingSelect, QOverload<int>::of(&KComboBox::activated),
            this, &ByteArrayUuencodingStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(encodingTypeLabel, mEncodingSelect);
}

ByteArrayUuencodingStreamEncoderConfigEditor::~ByteArrayUuencodingStreamEncoderConfigEditor() = default;

AbstractSelectionView* ByteArrayUuencodingStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview(mEncoder);
}

void ByteArrayUuencodingStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.algorithmId = static_cast<UuencodingStreamEncoderSettings::AlgorithmId>(mEncodingSelect->currentIndex());
    mSettings.fileName = mFileNameEdit->text();

    mEncoder->setSettings(mSettings);
}

}

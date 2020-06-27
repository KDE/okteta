/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayxxencodingstreamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>

namespace Kasten {

ByteArrayXxencodingStreamEncoderConfigEditor::ByteArrayXxencodingStreamEncoderConfigEditor(ByteArrayXxencodingStreamEncoder* encoder, QWidget* parent)
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
    connect(mFileNameEdit, &QLineEdit::textChanged, this, &ByteArrayXxencodingStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(fileNameLabel, mFileNameEdit);
}

ByteArrayXxencodingStreamEncoderConfigEditor::~ByteArrayXxencodingStreamEncoderConfigEditor() = default;

AbstractSelectionView* ByteArrayXxencodingStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview(mEncoder);
}

void ByteArrayXxencodingStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.fileName = mFileNameEdit->text();

    mEncoder->setSettings(mSettings);
}

}

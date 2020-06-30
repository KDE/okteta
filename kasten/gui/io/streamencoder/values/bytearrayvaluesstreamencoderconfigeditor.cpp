/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayvaluesstreamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
// #include <KComboBox>
// Qt
#include <QLayout>
#include <QLabel>
#include <QLineEdit>

namespace Kasten {

ByteArrayValuesStreamEncoderConfigEditor::ByteArrayValuesStreamEncoderConfigEditor(ByteArrayValuesStreamEncoder* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , mEncoder(encoder)
{
    mSettings = mEncoder->settings();

    auto* pageLayout = new QGridLayout(this);   // unknown rows
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->setColumnStretch(0, 0);
    pageLayout->setColumnStretch(1, 0);

#if 0
    // data type
    QLabel* label = new QLabel(i18n("Value coding:"), this);
    pageLayout->addWidget(label, 0, 0, Qt::AlignRight);

    mValueCodingSelect = new KComboBox(this);
    QStringList list;
    list.append(i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format", "Hexadecimal"));
    list.append(i18nc("@item:inmenu encoding of the bytes as values in the decimal format",    "Decimal"));
    list.append(i18nc("@item:inmenu encoding of the bytes as values in the octal format",      "Octal"));
    list.append(i18nc("@item:inmenu encoding of the bytes as values in the binary format",     "Binary"));
    mValueCodingSelect->addItems(list);
    mValueCodingSelect->setCurrentIndex(mSettings.valueCoding);
    connect(mValueCodingSelect, SIGNAL(activated(int)), SLOT(onSettingsChanged()));
    pageLayout->addWidget(mValueCodingSelect, 0, 1);
#endif
    // separation string
    QLabel* label = new QLabel(i18nc("@label:textbox substring which separates the values", "Separation:"), this);
    pageLayout->addWidget(label, 0, 0, Qt::AlignRight);

    mSeparationEdit = new QLineEdit(this);
    mSeparationEdit->setClearButtonEnabled(true);
    mSeparationEdit->setText(mSettings.separation);
    connect(mSeparationEdit, &QLineEdit::textChanged, this, &ByteArrayValuesStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addWidget(mSeparationEdit, 0, 1);

    // finish
    pageLayout->setRowStretch(2, 10);
}

ByteArrayValuesStreamEncoderConfigEditor::~ByteArrayValuesStreamEncoderConfigEditor() = default;

AbstractSelectionView* ByteArrayValuesStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview(mEncoder);
}

void ByteArrayValuesStreamEncoderConfigEditor::onSettingsChanged()
{
//     mSettings.valueCoding = (Okteta::ValueCoding) mValueCodingSelect->currentIndex();
    mSettings.separation = mSeparationEdit->text();

    mEncoder->setSettings(mSettings);
}

}

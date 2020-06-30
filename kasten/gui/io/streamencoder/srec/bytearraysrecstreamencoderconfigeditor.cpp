/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraysrecstreamencoderconfigeditor.hpp"

// lib
#include "bytearraytextstreamencoderpreview.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>
#include <QLabel>

namespace Kasten {

ByteArraySRecStreamEncoderConfigEditor::ByteArraySRecStreamEncoderConfigEditor(ByteArraySRecStreamEncoder* encoder, QWidget* parent)
    : AbstractModelStreamEncoderConfigEditor(parent)
    , mEncoder(encoder)
{
    mSettings = mEncoder->settings();

    auto* pageLayout = new QFormLayout(this);
    pageLayout->setContentsMargins(0, 0, 0, 0);

    // data type
    const QString addressSizeLabel =
        i18nc("@label:listbox the size in bits of the addresses.",
              "Address size:");

    mAddressSizeSelect = new KComboBox(this);
    const QStringList list {
        i18nc("@item:inmenu address size",
              "32-bit"),
        i18nc("@item:inmenu address size",
              "24-bit"),
        i18nc("@item:inmenu address size",
              "16-bit"),
    };
    mAddressSizeSelect->addItems(list);
    mAddressSizeSelect->setCurrentIndex(static_cast<int>(mSettings.addressSizeId));
    connect(mAddressSizeSelect, QOverload<int>::of(&KComboBox::activated),
            this, &ByteArraySRecStreamEncoderConfigEditor::onSettingsChanged);
    pageLayout->addRow(addressSizeLabel, mAddressSizeSelect);
}

ByteArraySRecStreamEncoderConfigEditor::~ByteArraySRecStreamEncoderConfigEditor() = default;

AbstractSelectionView* ByteArraySRecStreamEncoderConfigEditor::createPreviewView() const
{
    return new ByteArrayTextStreamEncoderPreview(mEncoder);
}

void ByteArraySRecStreamEncoderConfigEditor::onSettingsChanged()
{
    mSettings.addressSizeId = static_cast<SRecStreamEncoderSettings::AddressSizeId>(mAddressSizeSelect->currentIndex());

    mEncoder->setSettings(mSettings);
}

}

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsumbytearraychecksumparametersetedit.hpp"

// parameterset
#include "crc64bytearraychecksumparameterset.hpp"
#include "crc64bytearraychecksumparametersetedit.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>

const char Crc64ByteArrayChecksumParameterSetEdit::Id[] = "Crc64";

Crc64ByteArrayChecksumParameterSetEdit::Crc64ByteArrayChecksumParameterSetEdit(QWidget* parent)
    : AbstractByteArrayChecksumParameterSetEdit(parent)
{
    auto* baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mVariantComboBox = new KComboBox(this);
    //mVariantComboBox->addItem(i18nc("@item:inlistbox", "Custom"));
    mVariantComboBox->addItem(i18nc("@item:inlistbox", "ECMA-182"));
    mVariantComboBox->addItem(i18nc("@item:inlistbox", "ISO 3309"));
    connect(mVariantComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &Crc64ByteArrayChecksumParameterSetEdit::valuesChanged);

    const QString byteOrderLabelText =
        i18nc("@label:listbox variant used to select the CRC-64 variant",
              "CRC-64 variant:");
    const QString groupSizeToolTip =
        i18nc("@info:tooltip",
              "CRC-64 variant used by calculation.");
    mVariantComboBox->setToolTip(groupSizeToolTip);

    baseLayout->addRow(byteOrderLabelText, mVariantComboBox);
}

Crc64ByteArrayChecksumParameterSetEdit::~Crc64ByteArrayChecksumParameterSetEdit() = default;

bool Crc64ByteArrayChecksumParameterSetEdit::isValid() const { return true; }

void Crc64ByteArrayChecksumParameterSetEdit::setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet)
{
    const auto* crc64ParameterSet =
        static_cast<const Crc64ByteArrayChecksumParameterSet*>(parameterSet);

    mVariantComboBox->setCurrentIndex(static_cast<int>(crc64ParameterSet->crc64Variant()));
}

void Crc64ByteArrayChecksumParameterSetEdit::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    auto* crc64ParameterSet =
        static_cast<Crc64ByteArrayChecksumParameterSet*>(parameterSet);

    crc64ParameterSet->setCrc64Variant(static_cast<Crc64Variant>(mVariantComboBox->currentIndex()));
}

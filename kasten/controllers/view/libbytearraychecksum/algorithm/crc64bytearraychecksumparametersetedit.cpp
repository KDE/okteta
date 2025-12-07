/*
    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "crc64bytearraychecksumparametersetedit.hpp"

// parameterset
#include "crc64bytearraychecksumparameterset.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>

Crc64ByteArrayChecksumParameterSetEdit::Crc64ByteArrayChecksumParameterSetEdit(QWidget* parent)
    : AbstractByteArrayChecksumParameterSetEdit(parent)
{
    auto* const baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mVariantComboBox = new KComboBox(this);
    mVariantComboBox->addItem(i18nc("@item:inlistbox", "ECMA-182"));
    mVariantComboBox->addItem(i18nc("@item:inlistbox", "ISO 3309"));
    connect(mVariantComboBox, &KComboBox::activated,
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
    const auto* const crc64ParameterSet =
        static_cast<const Crc64ByteArrayChecksumParameterSet*>(parameterSet);

    mVariantComboBox->setCurrentIndex(static_cast<int>(crc64ParameterSet->variant()));
}

void Crc64ByteArrayChecksumParameterSetEdit::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    auto* const crc64ParameterSet =
        static_cast<Crc64ByteArrayChecksumParameterSet*>(parameterSet);

    crc64ParameterSet->setVariant(static_cast<Crc64ByteArrayChecksumParameterSet::Variant>(mVariantComboBox->currentIndex()));
}

#include "moc_crc64bytearraychecksumparametersetedit.cpp"

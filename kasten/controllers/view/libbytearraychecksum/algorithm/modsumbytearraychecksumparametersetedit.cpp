/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modsumbytearraychecksumparametersetedit.hpp"

// parameterset
#include "modsumbytearraychecksumparameterset.hpp"
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QFormLayout>

const char ModSumByteArrayChecksumParameterSetEdit::Id[] = "ModSum";

ModSumByteArrayChecksumParameterSetEdit::ModSumByteArrayChecksumParameterSetEdit(QWidget* parent)
    : AbstractByteArrayChecksumParameterSetEdit(parent)
{
    auto* baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mByteOrderComboBox = new KComboBox(this);
    mByteOrderComboBox->addItem(i18nc("@item:inlistbox", "Little-endian"));  // add first for index
    mByteOrderComboBox->addItem(i18nc("@item:inlistbox", "Big-endian"));     // add second for index
    connect(mByteOrderComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &ModSumByteArrayChecksumParameterSetEdit::valuesChanged);

    const QString byteOrderLabelText =
        i18nc("@label:listbox byte order to use for decoding the bytes into integer values",
              "Byte Order:");
    const QString groupSizeToolTip =
        i18nc("@info:tooltip",
              "The byte order to use for decoding the bytes into integer values.");
    mByteOrderComboBox->setToolTip(groupSizeToolTip);

    baseLayout->addRow(byteOrderLabelText, mByteOrderComboBox);
}

ModSumByteArrayChecksumParameterSetEdit::~ModSumByteArrayChecksumParameterSetEdit() = default;

bool ModSumByteArrayChecksumParameterSetEdit::isValid() const { return true; }

void ModSumByteArrayChecksumParameterSetEdit::setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet)
{
    const auto* modSumParameterSet =
        static_cast<const ModSumByteArrayChecksumParameterSet*>(parameterSet);

    mByteOrderComboBox->setCurrentIndex(modSumParameterSet->endianness());
}

void ModSumByteArrayChecksumParameterSetEdit::getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const
{
    auto* modSumParameterSet =
        static_cast<ModSumByteArrayChecksumParameterSet*>(parameterSet);

    modSumParameterSet->setEndianness(static_cast<Endianness>(mByteOrderComboBox->currentIndex()));
}

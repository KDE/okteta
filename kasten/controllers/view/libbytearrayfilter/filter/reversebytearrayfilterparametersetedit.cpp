/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "reversebytearrayfilterparametersetedit.hpp"

//
#include "reversebytearrayfilterparameterset.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QFormLayout>

const char ReverseByteArrayFilterParameterSetEdit::Id[] = "Reverse";

ReverseByteArrayFilterParameterSetEdit::ReverseByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{
    auto* baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    const QString invertsBitsLabelText =
        i18nc("@option:check",
              "Reverse also bits:");
    mInvertsBitsCheckBox = new QCheckBox(this);
    mInvertsBitsCheckBox->setChecked(false);
    const QString alignWhatsThis =
        i18nc("@info:whatsthis",
              "If set, the bits are arranged in reverse order as well.");
    mInvertsBitsCheckBox->setWhatsThis(alignWhatsThis);

    baseLayout->addRow(invertsBitsLabelText, mInvertsBitsCheckBox);
}

ReverseByteArrayFilterParameterSetEdit::~ReverseByteArrayFilterParameterSetEdit() = default;

void ReverseByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    const auto* reverseParameterSet = static_cast<const ReverseByteArrayFilterParameterSet*>(parameterSet);

    mInvertsBitsCheckBox->setChecked(reverseParameterSet->invertsBits());
}

void ReverseByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    auto* reverseParameterSet = static_cast<ReverseByteArrayFilterParameterSet*>(parameterSet);

    reverseParameterSet->setInvertsBits(mInvertsBitsCheckBox->isChecked());
}

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "rotatebytearrayfilterparametersetedit.hpp"

// parameterset
#include "rotatebytearrayfilterparameterset.hpp"
// KF
#include <KLocalizedString>
// Qt
#include <QFormLayout>
#include <QSpinBox>

const char RotateByteArrayFilterParameterSetEdit::Id[] = "Rotate";

RotateByteArrayFilterParameterSetEdit::RotateByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{
    auto* baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mGroupSizeEdit = new QSpinBox(this);
    mGroupSizeEdit->setRange(1, INT_MAX);

    const QString groupSizeLabelText =
        i18nc("@label:spinbox number of bytes the movement is done within",
              "&Group size (bytes):");
    const QString groupSizeToolTip =
        i18nc("@info:tooltip",
              "The number of bytes within which each movement is made.");
    mGroupSizeEdit->setToolTip(groupSizeToolTip);
    const QString groupSizeWhatsThis =
        i18nc("@info:whatsthis",
              "Control the number of bytes within which each movement is made.");
    mGroupSizeEdit->setWhatsThis(groupSizeWhatsThis);

    baseLayout->addRow(groupSizeLabelText, mGroupSizeEdit);

    mMoveBitWidthEdit = new QSpinBox(this);
    mMoveBitWidthEdit->setRange(INT_MIN, INT_MAX);
    connect(mMoveBitWidthEdit, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &RotateByteArrayFilterParameterSetEdit::onValueChanged);

    const QString moveBitWidthLabelText =
        i18nc("@label:spinbox width (in number of bits) the bits are moved",
              "S&hift width (bits):");
    const QString moveBitWidthToolTip =
        i18nc("@info:tooltip",
              "The width of the shift. Positive numbers move the bits to the right, negative to the left.");
    mMoveBitWidthEdit->setToolTip(moveBitWidthToolTip);
    const QString moveBitWidthWhatsThis =
        i18nc("@info:whatsthis",
              "Control the width of the shift. Positive numbers move the bits to the right, negative to the left.");
    mMoveBitWidthEdit->setWhatsThis(moveBitWidthWhatsThis);

    baseLayout->addRow(moveBitWidthLabelText, mMoveBitWidthEdit);
}

RotateByteArrayFilterParameterSetEdit::~RotateByteArrayFilterParameterSetEdit() = default;

bool RotateByteArrayFilterParameterSetEdit::isValid() const { return mMoveBitWidthEdit->value() != 0; }

void RotateByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    const auto* rotateParameterSet = static_cast<const RotateByteArrayFilterParameterSet*>(parameterSet);

    mGroupSizeEdit->setValue(rotateParameterSet->groupSize());
    mMoveBitWidthEdit->setValue(rotateParameterSet->moveBitWidth());
}

void RotateByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    auto* rotateParameterSet = static_cast<RotateByteArrayFilterParameterSet*>(parameterSet);

    rotateParameterSet->setGroupSize(mGroupSizeEdit->value());
    rotateParameterSet->setMoveBitWidth(mMoveBitWidthEdit->value());
}

void RotateByteArrayFilterParameterSetEdit::onValueChanged(int value)
{
    emit validityChanged(value != 0);
}

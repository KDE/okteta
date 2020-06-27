/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "operandbytearrayfilterparametersetedit.hpp"

//
#include "operandbytearrayfilterparameterset.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// KF
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QFormLayout>

const char OperandByteArrayFilterParameterSetEdit::Id[] = "Operand";

OperandByteArrayFilterParameterSetEdit::OperandByteArrayFilterParameterSetEdit(QWidget* parent)
    : AbstractByteArrayFilterParameterSetEdit(parent)
{
    auto* baseLayout = new QFormLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    const QString operandLabelText =
        i18nc("@label:textbox operand to the arithmetic filter function",
              "Operand:");
    mOperandEdit = new Okteta::ByteArrayComboBox(this);
    connect(mOperandEdit, &Okteta::ByteArrayComboBox::byteArrayChanged,
            this, &OperandByteArrayFilterParameterSetEdit::onInputChanged);
    const QString operandToolTip =
        i18nc("@info:tooltip",
              "The operand to do the operation with.");
    const QString operandWhatsThis =
        i18nc("@info:whatsthis",
              "Enter an operand, or select a previous operand from the list.");
    mOperandEdit->setToolTip(operandToolTip);
    mOperandEdit->setWhatsThis(operandWhatsThis);

    baseLayout->addRow(operandLabelText, mOperandEdit);

    const QString alignAtEndLabelText =
        i18nc("@option:check",
              "Align at end:");
    mAlignAtEndCheckBox = new QCheckBox(this);
    mAlignAtEndCheckBox->setChecked(false);
    const QString alignToolTip =
        i18nc("@info:tooltip",
              "Sets if the operation will be aligned to the end of the data instead of to the begin.");
    const QString alignWhatsThis =
        i18nc("@info:whatsthis",
              "If set, the operation will be aligned to the end of the data.");
    mAlignAtEndCheckBox->setToolTip(alignToolTip);
    mAlignAtEndCheckBox->setWhatsThis(alignWhatsThis);

    baseLayout->addRow(alignAtEndLabelText, mAlignAtEndCheckBox);
}

OperandByteArrayFilterParameterSetEdit::~OperandByteArrayFilterParameterSetEdit() = default;

bool OperandByteArrayFilterParameterSetEdit::isValid() const { return !mOperandEdit->byteArray().isEmpty(); }

void OperandByteArrayFilterParameterSetEdit::setValues(const AbstractByteArrayFilterParameterSet* parameterSet)
{
    const auto* operandParameterSet = static_cast<const OperandByteArrayFilterParameterSet*>(parameterSet);

//     mOperandEdit->setValue( operandParameterSet->operand() ); TODO: not yet implemented
    mAlignAtEndCheckBox->setChecked(operandParameterSet->alignAtEnd());
}

void OperandByteArrayFilterParameterSetEdit::setCharCodec(const QString& charCodecName)
{
    mOperandEdit->setCharCodec(charCodecName);
}

void OperandByteArrayFilterParameterSetEdit::getParameterSet(AbstractByteArrayFilterParameterSet* parameterSet) const
{
    auto* operandParameterSet = static_cast<OperandByteArrayFilterParameterSet*>(parameterSet);

    operandParameterSet->setOperand(mOperandEdit->byteArray());
    operandParameterSet->setOperandFormat(mOperandEdit->format());
    operandParameterSet->setAlignAtEnd(mAlignAtEndCheckBox->isChecked());
}

void OperandByteArrayFilterParameterSetEdit::rememberCurrentSettings()
{
    mOperandEdit->rememberCurrentByteArray();
}

void OperandByteArrayFilterParameterSetEdit::onInputChanged(const QByteArray& data)
{
    emit validityChanged(!data.isEmpty());
}

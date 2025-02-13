/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofileedit.hpp"

// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfile>
// Okteta core
#include <Okteta/CharCodec>
// KF
#include <KLocalizedString>
#include <KComboBox>
// Qt
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
// Std
#include <limits>

namespace Kasten {

static constexpr int
listIndexFromByteArrayCodingsFlags(int byteArrayCodingsFlags)
{
    return byteArrayCodingsFlags - 1;
}

static constexpr int
byteArrayCodingsFlagsFromListIndex(int listIndex)
{
    return listIndex + 1;
}

ViewProfileEdit::ViewProfileEdit(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // titel
    auto* titleFormLayout = new QFormLayout;
    // char for non-printable bytes
    mTitleEdit = new QLineEdit(this);
    mTitleEdit->setClearButtonEnabled(true);
    connect(mTitleEdit, &QLineEdit::textChanged,
            this, &ViewProfileEdit::profileTitleChanged);
    titleFormLayout->addRow(i18nc("@label:textbox", "Title:"), mTitleEdit);

    // display settings
    auto* displayBox = new QGroupBox(this);
    displayBox->setTitle(i18nc("@title:group", "Display"));
    auto* displayBoxFormLayout = new QFormLayout(displayBox);
    // line offset shown
    mLineOffsetShownCheckBox = new QCheckBox(displayBox);
    displayBoxFormLayout->addRow(i18nc("@option:check", "Show line offset:"), mLineOffsetShownCheckBox);
    // offset coding
    mOffsetCodingComboBox = new KComboBox(displayBox);
    const QStringList offsetCodingList {
        i18nc("@item:inmenu offset in the hexadecimal format", "Hexadecimal"),
        i18nc("@item:inmenu offset in the decimal format", "Decimal"),
        i18nc("@item:inmenu offset in the octal format", "Octal"),
    };
    mOffsetCodingComboBox->addItems(offsetCodingList);
    displayBoxFormLayout->addRow(i18nc("@label:listbox", "Offset coding:"), mOffsetCodingComboBox);
    // values or char shown
    mValuesCharsShownComboBox = new KComboBox(displayBox);
    const QStringList valuesCharsList {
        i18nc("@item:inmenu", "Values"),
        i18nc("@item:inmenu", "Chars"),
        i18nc("@item:inmenu", "Values & Chars"),
    };
    mValuesCharsShownComboBox->addItems(valuesCharsList);
    displayBoxFormLayout->addRow(i18nc("@option:check", "Show values or chars:"), mValuesCharsShownComboBox);
    // display mode
    const QString displayModeLabel =
        i18nc("@label:listbox ",
              "Show with rows or columns:");
    mDisplayModeComboBox = new KComboBox(displayBox);
    const QStringList displayModeList {
        i18nc("@item:inmenu", "Columns"),
        i18nc("@item:inmenu", "Rows"),
    };
    mDisplayModeComboBox->addItems(displayModeList);
    displayBoxFormLayout->addRow(displayModeLabel, mDisplayModeComboBox);

    // layout settings
    auto* layoutBox = new QGroupBox(this);
    layoutBox->setTitle(i18nc("@title:group", "Layout"));
    auto* layoutBoxFormLayout = new QFormLayout(layoutBox);
    // line break
    mLineBreakComboBox = new KComboBox(layoutBox);
    const QStringList lineBreakList {
        i18nc("@item:inmenu  The layout will not change on size changes.",
              "Off"),
        i18nc("@item:inmenu  The layout will adapt to the size, but only with complete groups of bytes.",
              "Wrap Only Complete Byte Groups"),
        i18nc("@item:inmenu  The layout will adapt to the size and fit in as much bytes per line as possible.",
              "On"),
    };
    mLineBreakComboBox->addItems(lineBreakList);
    connect(mLineBreakComboBox, qOverload<int>(&KComboBox::currentIndexChanged),
            this, &ViewProfileEdit::onLineBreakIndexChanged);
    layoutBoxFormLayout->addRow(i18nc("@label:listbox", "Break lines:"), mLineBreakComboBox);
    // bytes per group
    mGroupedBytesCountEdit = new QSpinBox(this);
    mGroupedBytesCountEdit->setRange(0, std::numeric_limits<int>::max());
    const QString noGroupingText = i18nc("@item", "No Grouping");
    mGroupedBytesCountEdit->setSpecialValueText(noGroupingText);
    const QString groupedBytesCountLabel =
        i18nc("@label:spinbox number of bytes which are grouped",
              "Bytes per group:");
    layoutBoxFormLayout->addRow(groupedBytesCountLabel, mGroupedBytesCountEdit);
    // bytes per group
    mBytesPerLineEdit = new QSpinBox(this);
    mBytesPerLineEdit->setRange(1, std::numeric_limits<int>::max());
    const QString bytesPerLineLabel =
        i18nc("@label:spinbox number of bytes which are shown per line",
              "Bytes per line:");
    layoutBoxFormLayout->addRow(bytesPerLineLabel, mBytesPerLineEdit);

    // value settings
    auto* valuesBox = new QGroupBox(this);
    valuesBox->setTitle(i18nc("@title:group", "Values"));
    auto* valuesBoxFormLayout = new QFormLayout(valuesBox);
    // coding
    mValueCodingComboBox = new KComboBox(valuesBox);
    const QStringList valueCodingList {
        i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format",
              "Hexadecimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the decimal format",
              "Decimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the octal format",
              "Octal"),
        i18nc("@item:inmenu encoding of the bytes as values in the binary format",
              "Binary"),
    };
    mValueCodingComboBox->addItems(valueCodingList);
    valuesBoxFormLayout->addRow(i18nc("@label:listbox", "Coding:"), mValueCodingComboBox);

    // char settings
    auto* charsBox = new QGroupBox(this);
    charsBox->setTitle(i18nc("@title:group", "Chars"));
    auto* charsBoxFormLayout = new QFormLayout(charsBox);
    // coding
    mCharCodingComboBox = new KComboBox(charsBox);
    mCharCodingComboBox->addItems(Okteta::CharCodec::codecNames());
    charsBoxFormLayout->addRow(i18nc("@label:listbox", "Coding:"), mCharCodingComboBox);
    // line offset shown
    mNonPrintableShownCheckBox = new QCheckBox(charsBox);
    // TODOSHOWNONPRINTING hide from UI for now
    mNonPrintableShownCheckBox->hide();
    // charsBoxFormLayout->addRow(i18nc("@option:check", "Show Non-printable:"), mNonPrintableShownCheckBox);
    // char for non-printable bytes
    mNonPrintableCharEdit = new QLineEdit(charsBox);   // TODO: use a validator to ensure always one char
    mNonPrintableCharEdit->setClearButtonEnabled(true);
    mNonPrintableCharEdit->setMaxLength(1);
    charsBoxFormLayout->addRow(i18nc("@label:chooser", "Char for non-printable bytes:"), mNonPrintableCharEdit);
    // char for undefined bytes
    mUndefinedCharEdit = new QLineEdit(charsBox);   // TODO: use a validator to ensure always one char
    mUndefinedCharEdit->setClearButtonEnabled(true);
    mUndefinedCharEdit->setMaxLength(1);
    charsBoxFormLayout->addRow(i18nc("@label:chooser", "Char for undefined bytes:"), mUndefinedCharEdit);

    layout->addLayout(titleFormLayout);
    layout->addWidget(displayBox);
    layout->addWidget(layoutBox);
    layout->addWidget(valuesBox);
    layout->addWidget(charsBox);

    mTitleEdit->setFocus();
}

ViewProfileEdit::~ViewProfileEdit() = default;

ByteArrayViewProfile ViewProfileEdit::viewProfile() const
{
    ByteArrayViewProfile viewProfile;
    viewProfile.setViewProfileTitle(mTitleEdit->text());

    viewProfile.setOffsetColumnVisible(mLineOffsetShownCheckBox->isChecked());
    viewProfile.setOffsetCoding(mOffsetCodingComboBox->currentIndex());
    const int visibleByteArrayCodings =
        byteArrayCodingsFlagsFromListIndex(mValuesCharsShownComboBox->currentIndex());
    viewProfile.setVisibleByteArrayCodings(visibleByteArrayCodings);
    viewProfile.setViewModus(mDisplayModeComboBox->currentIndex());

    viewProfile.setLayoutStyle(mLineBreakComboBox->currentIndex());
    viewProfile.setNoOfGroupedBytes(mGroupedBytesCountEdit->value());
    viewProfile.setNoOfBytesPerLine(mBytesPerLineEdit->value());

    viewProfile.setValueCoding(mValueCodingComboBox->currentIndex());

    viewProfile.setCharCoding(mCharCodingComboBox->currentText());
    viewProfile.setShowsNonprinting(mNonPrintableShownCheckBox->isChecked());
    viewProfile.setSubstituteChar(mNonPrintableCharEdit->text().at(0));   // TODO: need make sure is one char
    viewProfile.setUndefinedChar(mUndefinedCharEdit->text().at(0));   // TODO: need make sure is one char

    return viewProfile;
}

void ViewProfileEdit::setViewProfile(const ByteArrayViewProfile& viewProfile)
{
    mTitleEdit->setText(viewProfile.viewProfileTitle());

    mLineOffsetShownCheckBox->setChecked(viewProfile.offsetColumnVisible());
    mOffsetCodingComboBox->setCurrentIndex(viewProfile.offsetCoding());
    const int valuesCharsShownListIndex =
        listIndexFromByteArrayCodingsFlags(viewProfile.visibleByteArrayCodings());
    mValuesCharsShownComboBox->setCurrentIndex(valuesCharsShownListIndex);
    mDisplayModeComboBox->setCurrentIndex(viewProfile.viewModus());

    mLineBreakComboBox->setCurrentIndex(viewProfile.layoutStyle());
    mGroupedBytesCountEdit->setValue(viewProfile.noOfGroupedBytes());
    mBytesPerLineEdit->setValue(viewProfile.noOfBytesPerLine());

    mValueCodingComboBox->setCurrentIndex(viewProfile.valueCoding());

    mCharCodingComboBox->setCurrentItem(viewProfile.charCodingName());
    mNonPrintableShownCheckBox->setChecked(viewProfile.showsNonprinting());
    mNonPrintableCharEdit->setText(viewProfile.substituteChar());
    mUndefinedCharEdit->setText(viewProfile.undefinedChar());
}

void ViewProfileEdit::onLineBreakIndexChanged(int lineBreakIndex)
{
    const bool isLineBreakByByte = (lineBreakIndex == 0);

    mBytesPerLineEdit->setEnabled(isLineBreakByByte);
}

}

#include "moc_viewprofileedit.cpp"

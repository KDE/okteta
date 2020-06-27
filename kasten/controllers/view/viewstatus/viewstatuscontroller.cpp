/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewstatuscontroller.hpp"

// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayView>
// Kasten ui
#include <Kasten/ToggleButton>
#include <Kasten/StatusBar>
// Okteta core
#include <Okteta/CharCodec>
#include <Okteta/OktetaCore>
// KF
#include <KSqueezedTextLabel>
#include <KComboBox>
#include <KLocalizedString>
// Qt
#include <QLayout>
#include <QLabel>
#include <QFontMetrics>

// TODO: make status bar capable to hide entries if size is too small, use priorisation

namespace Kasten {

ViewStatusController::ViewStatusController(StatusBar* statusBar)
    : mStatusBar(statusBar)
{
    mPrintFunction = Okteta::OffsetFormat::printFunction(Okteta::OffsetFormat::Hexadecimal);

    mOffsetLabel = new QLabel(statusBar);
    statusBar->addWidget(mOffsetLabel);

    mSelectionLabel = new QLabel(statusBar);
    statusBar->addWidget(mSelectionLabel);

    const QString insertModeText = i18nc("@info:status short for: Insert mode",    "INS");
    const QString overwriteModeText = i18nc("@info:status short for: Overwrite mode", "OVR");
    const QString insertModeTooltip = i18nc("@info:tooltip", "Insert mode");
    const QString overwriteModeTooltip = i18nc("@info:tooltip", "Overwrite mode");
    mOverwriteModeToggleButton = new ToggleButton(insertModeText, insertModeTooltip, statusBar);
    mOverwriteModeToggleButton->setCheckedState(overwriteModeText, overwriteModeTooltip);
    statusBar->addWidget(mOverwriteModeToggleButton);
    connect(mOverwriteModeToggleButton, &ToggleButton::clicked, this, &ViewStatusController::setOverwriteMode);

    mValueCodingComboBox = new KComboBox(statusBar);
    const QStringList list {
        i18nc("@item:inmenu encoding of the bytes as values in the hexadecimal format", "Hexadecimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the decimal format",     "Decimal"),
        i18nc("@item:inmenu encoding of the bytes as values in the octal format",       "Octal"),
        i18nc("@item:inmenu encoding of the bytes as values in the binary format",      "Binary"),
    };
    mValueCodingComboBox->addItems(list);
    mValueCodingComboBox->setToolTip(
        i18nc("@info:tooltip", "Coding of the value interpretation in the current view."));
    connect(mValueCodingComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &ViewStatusController::setValueCoding);
    statusBar->addWidget(mValueCodingComboBox);

    mCharCodingComboBox = new KComboBox(statusBar);
    mCharCodingComboBox->addItems(Okteta::CharCodec::codecNames());
    mCharCodingComboBox->setToolTip(
        i18nc("@info:tooltip", "Encoding in the character column of the current view."));
    connect(mCharCodingComboBox, QOverload<int>::of(&KComboBox::activated),
            this, &ViewStatusController::setCharCoding);
    statusBar->addWidget(mCharCodingComboBox);

    fixWidths(Okteta::OffsetFormat::Hexadecimal);

    setTargetModel(nullptr);
}

// the letter C can be very wide, that is why with proportional fonts there seems too much space used, but isn't
// see https://frinring.wordpress.com/2008/10/14/better-width-with-open-sources/
void ViewStatusController::fixWidths(int offsetCoding)
{
    const QFontMetrics metrics = mStatusBar->fontMetrics();

    // mOffsetLabel
    constexpr int hexDigitsCount = 16;
    constexpr int decimalDigitsCount = 10;
    constexpr int firstLetterIndex = 10;
    constexpr char digits[hexDigitsCount] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'A', 'B', 'C', 'D', 'E', 'F'
    };

    int largestOffsetWidth = 0;
    int largestSelectionWidth = 0;
    int widestDigitIndex = 0;
    const int digitsCount = (offsetCoding == Okteta::OffsetFormat::Hexadecimal) ? hexDigitsCount : decimalDigitsCount;
    for (int i = 0; i < digitsCount; ++i) {
        QString offset;
        if (offsetCoding == Okteta::OffsetFormat::Hexadecimal) {
            offset = QString(9, QLatin1Char(digits[i]));
            offset[4] = QLatin1Char(':');
        } else {
            offset = QString(10, QLatin1Char(digits[i]));
        }

        const QString offsetText = i18n("Offset: %1", offset);
        const int offsetWidth = metrics.boundingRect(offsetText).width();
        if (largestOffsetWidth < offsetWidth) {
            largestOffsetWidth = offsetWidth;
        }

        const char countDigit = (i < firstLetterIndex) ? digits[i] : digits[widestDigitIndex];
        const int maxNumber = QByteArray('1' + QByteArray(9, countDigit)).toInt();
        const QString bytesCount = i18n("%1 bytes", maxNumber);
        const QString selectionString = i18nc("@info:status selection: start offset - end offset ()",
                                              "Selection: %1 - %2 (%3)", offset, offset, bytesCount);

        const int selectionWidth = metrics.boundingRect(selectionString).width();
        if (largestSelectionWidth < selectionWidth) {
            if (i < firstLetterIndex) {
                widestDigitIndex = i;
            }
            largestSelectionWidth = selectionWidth;
        }
    }

    mOffsetLabel->setFixedWidth(largestOffsetWidth);
    mSelectionLabel->setFixedWidth(largestSelectionWidth);
    mStatusBar->updateLayout();
}

void ViewStatusController::setTargetModel(AbstractModel* model)
{
    if (mByteArrayView) {
        mByteArrayView->disconnect(this);
        mByteArrayView->disconnect(mOverwriteModeToggleButton);
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : nullptr;

    const bool hasView = (mByteArrayView != nullptr);
    if (hasView) {
        mStartOffset = mByteArrayView->startOffset();

        onCursorPositionChanged(mByteArrayView->cursorPosition());
        onSelectedDataChanged(mByteArrayView->modelSelection());
        mOverwriteModeToggleButton->setChecked(mByteArrayView->isOverwriteMode());
        onOffsetCodingChanged(mByteArrayView->offsetCoding());
        onValueCodingChanged(mByteArrayView->valueCoding());
        onCharCodecChanged(mByteArrayView->charCodingName());

        connect(mByteArrayView, &ByteArrayView::cursorPositionChanged, this, &ViewStatusController::onCursorPositionChanged);
        connect(mByteArrayView, &ByteArrayView::selectedDataChanged,
                this, &ViewStatusController::onSelectedDataChanged);
        connect(mByteArrayView, &ByteArrayView::overwriteModeChanged,
                mOverwriteModeToggleButton, &ToggleButton::setChecked);
        connect(mByteArrayView, &ByteArrayView::offsetCodingChanged, this, &ViewStatusController::onOffsetCodingChanged);
        connect(mByteArrayView, &ByteArrayView::valueCodingChanged, this, &ViewStatusController::onValueCodingChanged);
        connect(mByteArrayView, &ByteArrayView::charCodecChanged,
                this, &ViewStatusController::onCharCodecChanged);
    } else {
        mOffsetLabel->setText(i18nc("@info:status offset value not available", "Offset: -"));
        mSelectionLabel->setText(i18nc("@info:status offset value not available", "Selection: -"));
        mOverwriteModeToggleButton->setChecked(false);
        mValueCodingComboBox->setCurrentIndex(0);
        mCharCodingComboBox->setCurrentIndex(0);
    }

    mOffsetLabel->setEnabled(hasView);
    mSelectionLabel->setEnabled(hasView);
    mOverwriteModeToggleButton->setEnabled(hasView);
    mValueCodingComboBox->setEnabled(hasView);
    mCharCodingComboBox->setEnabled(hasView);
}

void ViewStatusController::setOverwriteMode(bool overwrite)
{
    mByteArrayView->setOverwriteMode(overwrite);
}

void ViewStatusController::setValueCoding(int valueCoding)
{
    mByteArrayView->setValueCoding(valueCoding);
    mByteArrayView->setFocus();
}

void ViewStatusController::setCharCoding(int charCoding)
{
    mByteArrayView->setCharCoding(Okteta::CharCodec::codecNames()[charCoding]);
    mByteArrayView->setFocus();
}

void ViewStatusController::onCursorPositionChanged(Okteta::Address offset)
{
    char codedOffset[Okteta::OffsetFormat::MaxFormatWidth + 1];

    mPrintFunction(codedOffset, mStartOffset + offset);

    mOffsetLabel->setText(i18n("Offset: %1", QString::fromUtf8(codedOffset)));
}

// TODO: fix selection by cursor not sending updates
void ViewStatusController::onSelectedDataChanged(const Kasten::AbstractModelSelection* modelSelection)
{
    const auto* byteArraySelection = static_cast<const ByteArraySelection*>(modelSelection);
    const Okteta::AddressRange selection = byteArraySelection->range();

    QString selectionString;
    if (!selection.isEmpty()) {
        char codedSelectionStart[Okteta::OffsetFormat::MaxFormatWidth + 1];
        char codedSelectionEnd[Okteta::OffsetFormat::MaxFormatWidth + 1];

        mPrintFunction(codedSelectionStart, mStartOffset + selection.start());
        mPrintFunction(codedSelectionEnd,   mStartOffset + selection.end());

        const QString bytesCount = i18np("1 byte", "%1 bytes", selection.width());
        selectionString = i18nc("@info:status selection: start offset - end offset (number of bytes)",
                                "Selection: %1 - %2 (%3)", QString::fromUtf8(codedSelectionStart), QString::fromUtf8(codedSelectionEnd), bytesCount);
    } else {
        selectionString = i18nc("@info:status offset value not available", "Selection: -");
    }

    mSelectionLabel->setText(selectionString);
}

void ViewStatusController::onOffsetCodingChanged(int offsetCoding)
{
    mPrintFunction = Okteta::OffsetFormat::printFunction((Okteta::OffsetFormat::Format)offsetCoding);
    fixWidths(offsetCoding);

    // trigger updates of offset printing labels
    onCursorPositionChanged(mByteArrayView->cursorPosition());
    onSelectedDataChanged(mByteArrayView->modelSelection());
}

void ViewStatusController::onValueCodingChanged(int valueCoding)
{
    mValueCodingComboBox->setCurrentIndex(valueCoding);
}

void ViewStatusController::onCharCodecChanged(const QString& charCodecName)
{
    const int charCodingIndex = Okteta::CharCodec::codecNames().indexOf(charCodecName);

    mCharCodingComboBox->setCurrentIndex(charCodingIndex);
}

}

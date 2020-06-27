/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearraycombobox_p.hpp"
#include "bytearraycombobox.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QLayout>
#include <QLineEdit>
#include <QAbstractItemView>

namespace Okteta {

const QStringList& formatNames()
{
    static QStringList list = QStringList {
//         i18nc("@item:inlistbox guessing the coding of the bytes by the input",       "Auto"),
        i18nc("@item:inlistbox coding of the bytes as values in the hexadecimal format", "Hex"),
        i18nc("@item:inlistbox coding of the bytes as values in the decimal format",     "Dec"),
        i18nc("@item:inlistbox coding of the bytes as values in the octal format",       "Oct"),
        i18nc("@item:inlistbox coding of the bytes as values in the binary format",      "Bin"),
        i18nc("@item:inlistbox coding of the bytes as characters with the values",       "Char"),
        i18nc("@item:inlistbox coding of the bytes as UTF-8 characters with the values", "UTF-8"),
    };
    return list;
}

void ByteArrayComboBoxPrivate::init()
{
    Q_Q(ByteArrayComboBox);

    auto* baseLayout = new QHBoxLayout(q);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    mFormatComboBox = new KComboBox(q);
    mFormatComboBox->addItems(formatNames());
    QObject::connect(mFormatComboBox, QOverload<int>::of(&QComboBox::activated),
                     q, [&](int index) { onFormatChanged(index); });

    mValueComboBox = new KComboBox(q);
    mValueComboBox->setEditable(true);
    mValueComboBox->setMaxCount(10);
    mValueComboBox->setInsertPolicy(QComboBox::NoInsert);
    mValueComboBox->setDuplicatesEnabled(false);
    q->setFocusProxy(mValueComboBox);
    QObject::connect(mValueComboBox->lineEdit(), &QLineEdit::textEdited,
                     q, [&](const QString& text) { onValueEdited(text); });
    QAbstractItemView* formatComboBoxListView = mFormatComboBox->view();
    QObject::connect(formatComboBoxListView, &QAbstractItemView::activated,
                     mValueComboBox, QOverload<>::of(&KComboBox::setFocus));
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect(formatComboBoxListView, &QAbstractItemView::pressed,
                     mValueComboBox, QOverload<>::of(&KComboBox::setFocus));
    mValidator = new ByteArrayValidator(mValueComboBox);
    const ByteArrayValidator::Coding coding =
        static_cast<ByteArrayValidator::Coding>(mFormatComboBox->currentIndex());
    mValidator->setCodec(coding);

    mValueComboBox->setValidator(mValidator);
    QObject::connect(mValueComboBox, QOverload<int>::of(&QComboBox::activated),
                     q, [&](int index) { onValueActivated(index); });

    baseLayout->addWidget(mFormatComboBox);
    baseLayout->addWidget(mValueComboBox, 1);
    QWidget::setTabOrder(mFormatComboBox, mValueComboBox);
}

void ByteArrayComboBoxPrivate::setByteArray(const QByteArray& byteArray)
{
    mValueComboBox->setEditText(mValidator->toString(byteArray));
}

void ByteArrayComboBoxPrivate::setCharCodec(const QString& charCodecName)
{
    const bool isChar8Visible = (mFormatComboBox->currentIndex() == ByteArrayValidator::CharCoding);

    // update the char string if shown
    QByteArray currentByteArray;
    if (isChar8Visible) {
        const QString currentChar8String = mValueComboBox->currentText();
        currentByteArray = mValidator->toByteArray(currentChar8String);
    }

    mValidator->setCharCodec(charCodecName);

    if (isChar8Visible) {
        const QString char8String = mValidator->toString(currentByteArray);
        mValueComboBox->setEditText(char8String);
    }
}

void ByteArrayComboBoxPrivate::setMaxLength(int maxLength)
{
    const int oldMaxLength = mValidator->maxLength();
    if (oldMaxLength == maxLength) {
        return;
    }

    mValidator->setMaxLength(maxLength);

    if (oldMaxLength > maxLength) {
        QString currentText = mValueComboBox->currentText();
        int dummyPos;
        mValidator->validate(currentText, dummyPos);
        mValueComboBox->setEditText(currentText);
    }
}

void ByteArrayComboBoxPrivate::setMinLength(int minLength)
{
    const int oldMinLength = mValidator->minLength();
    if (oldMinLength == minLength) {
        return;
    }

    mValidator->setMinLength(minLength);

    if (oldMinLength < minLength) {
        QString currentText = mValueComboBox->currentText();
        int dummyPos;
        mValidator->validate(currentText, dummyPos);
        mValueComboBox->setEditText(currentText);
    }
}

void ByteArrayComboBoxPrivate::rememberCurrentByteArray()
{
    mValueComboBox->insertItem(-1, mValueComboBox->currentText(), mFormatComboBox->currentIndex());
}

int ByteArrayComboBoxPrivate::maxLength() const
{
    return mValidator->maxLength();
}

int ByteArrayComboBoxPrivate::minLength() const
{
    return mValidator->minLength();
}

void ByteArrayComboBoxPrivate::onFormatChanged(int index)
{
    Q_Q(ByteArrayComboBox);

    const QString currentValueText = mValueComboBox->currentText();
    const bool isCurrentValueTextEmpty = currentValueText.isEmpty();
    const QByteArray byteArray = isCurrentValueTextEmpty ? QByteArray() : mValidator->toByteArray(currentValueText);

    mValidator->setCodec(static_cast<ByteArrayValidator::Coding>(index));

    if (!isCurrentValueTextEmpty) {
        const QString convertedValueText = mValidator->toString(byteArray);
        mValueComboBox->setEditText(convertedValueText);
    }

    emit q->formatChanged(index);
}

void ByteArrayComboBoxPrivate::onValueEdited(const QString& value)
{
    Q_Q(ByteArrayComboBox);

    const QByteArray byteArray = mValidator->toByteArray(value);

    emit q->byteArrayChanged(byteArray);
}

void ByteArrayComboBoxPrivate::onValueActivated(int index)
{
    Q_Q(ByteArrayComboBox);

    if (index != -1) {
        const int oldFormatIndex = mFormatComboBox->currentIndex();
        const int itemFormatIndex = mValueComboBox->itemData(index).toInt();
        const bool isOtherFormat = (oldFormatIndex != itemFormatIndex);

        if (isOtherFormat) {
            mFormatComboBox->setCurrentIndex(itemFormatIndex);
            mValidator->setCodec(static_cast<ByteArrayValidator::Coding>(itemFormatIndex));

        }
        const QString currentValueText = mValueComboBox->currentText();
        const QByteArray byteArray = mValidator->toByteArray(currentValueText);

        emit q->byteArrayChanged(byteArray);
        if (isOtherFormat) {
            emit q->formatChanged(itemFormatIndex);
        }
    }
}

}

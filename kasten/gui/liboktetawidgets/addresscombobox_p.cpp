/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addresscombobox_p.hpp"
#include "addresscombobox.hpp"

// KF
#include <KLocalizedString>
// Qt
#include <QLayout>
#include <QLineEdit>
#include <QAbstractItemView>

/*
   Problem: what to do if the format is changed for which the current string is not valid?
   Solution: we always convert the string to the new format, so there is never such a situation
 */

namespace Okteta {

static const QStringList& formatStrings()
{
    static QStringList list = QStringList {
//         i18nc("@item:inlistbox guessing the format of the address by the input",      "Auto"),
        i18nc("@item:inlistbox coding of offset in the hexadecimal format", "Hex"),
        i18nc("@item:inlistbox coding of offset in the decimal format",     "Dec"),
        i18nc("@item:inlistbox coding of offset in the expression format",  "Expr"),
    };
    return list;
}

void AddressComboBoxPrivate::init()
{
    Q_Q(AddressComboBox);

    auto* baseLayout = new QHBoxLayout(q);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    mFormatComboBox = new KComboBox(q);
    mFormatComboBox->addItems(formatStrings());
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
    mValidator = new AddressValidator(mValueComboBox, AddressValidator::HexadecimalCoding);
    const AddressValidator::Coding coding =
        static_cast<AddressValidator::Coding>(mFormatComboBox->currentIndex());
    mValidator->setCodec(coding);
    mValueComboBox->setValidator(mValidator);
    QObject::connect(mValueComboBox, QOverload<int>::of(&QComboBox::activated),
                     q, [&](int index) { onValueActivated(index); });
    baseLayout->addWidget(mFormatComboBox);
    baseLayout->addWidget(mValueComboBox, 1);
    QWidget::setTabOrder(mFormatComboBox, mValueComboBox);
}

void AddressComboBoxPrivate::rememberCurrentAddress()
{
    // don't insert same value multiple times in a row
    if (mValueComboBox->itemText(0) != mValueComboBox->currentText()) {
        mValueComboBox->insertItem(-1, mValueComboBox->currentText(), mFormatComboBox->currentIndex());
    }
}

void AddressComboBoxPrivate::onFormatChanged(int formatIndex)
{
    Q_Q(AddressComboBox);

    const QString currentValueText = mValueComboBox->currentText();
    const bool isCurrentValueTextEmpty = currentValueText.isEmpty();

    AddressValidator::AddressType addressType;
    Address address;
    if (isCurrentValueTextEmpty) {
        address = -1;
        addressType = mAddressType;
    } else {
        address = mValidator->toAddress(currentValueText, &addressType);
    }

    mValidator->setCodec(static_cast<AddressValidator::Coding>(formatIndex));

    if (!isCurrentValueTextEmpty) {
        const QString convertedValueText = mValidator->toString(address, addressType);
        mValueComboBox->setEditText(convertedValueText);
    }

    if (mAddressType != addressType) {
        mAddressType = addressType;
        emit q->addressTypeChanged(mAddressType);
    }
    emit q->formatChanged(formatIndex);
}

void AddressComboBoxPrivate::onValueEdited(const QString& value)
{
    Q_Q(AddressComboBox);

    AddressValidator::AddressType addressType;
    const Address address = mValidator->toAddress(value, &addressType);

    if (mAddressType != addressType) {
        mAddressType = addressType;
        emit q->addressTypeChanged(mAddressType);
    }
    emit q->addressChanged(address);
}

void AddressComboBoxPrivate::onValueActivated(int index)
{
    Q_Q(AddressComboBox);

    if (index != -1) {
        const int oldFormatIndex = mFormatComboBox->currentIndex();
        const int itemFormatIndex = mValueComboBox->itemData(index).toInt();

        const bool isOtherFormat = (oldFormatIndex != itemFormatIndex);
        if (isOtherFormat) {
            mFormatComboBox->setCurrentIndex(itemFormatIndex);
            mValidator->setCodec(static_cast<AddressValidator::Coding>(itemFormatIndex));

        }
        const QString currentValueText = mValueComboBox->currentText();

        AddressValidator::AddressType addressType;
        const Address address = mValidator->toAddress(currentValueText, &addressType);

        if (mAddressType != addressType) {
            mAddressType = addressType;
            emit q->addressTypeChanged(mAddressType);
        }
        emit q->addressChanged(address);
        if (isOtherFormat) {
            emit q->formatChanged(itemFormatIndex);
        }
    }
}

}

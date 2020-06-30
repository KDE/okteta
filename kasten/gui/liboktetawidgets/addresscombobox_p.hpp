/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ADDRESSCOMBOBOX_P_HPP
#define KASTEN_ADDRESSCOMBOBOX_P_HPP

// lib
#include "addresscombobox.hpp"
#include "addressvalidator.hpp"
// KF
#include <KComboBox>

namespace Okteta {

class AddressComboBoxPrivate
{
public:
    explicit AddressComboBoxPrivate(AddressComboBox* parent);

public:
    Address address() const;
    int format() const;
    AddressValidator::AddressType addressType() const;

public:
    void init();
    void rememberCurrentAddress();

    void onFormatChanged(int index);
    void onValueEdited(const QString& value);
    void onValueActivated(int index);

private:
    AddressComboBox* const q_ptr;
    Q_DECLARE_PUBLIC(AddressComboBox)

    KComboBox * mFormatComboBox;
    KComboBox* mValueComboBox;
    AddressValidator::AddressType mAddressType;
    AddressValidator* mValidator;
};

inline AddressComboBoxPrivate::AddressComboBoxPrivate(AddressComboBox* parent)
    : q_ptr(parent)
{}

inline Address AddressComboBoxPrivate::address() const
{
    return mValidator->toAddress(mValueComboBox->currentText());
}
inline int AddressComboBoxPrivate::format() const
{
    return mFormatComboBox->currentIndex();
}
inline AddressValidator::AddressType AddressComboBoxPrivate::addressType() const
{
    return mAddressType;
}

}

#endif

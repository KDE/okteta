/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "addresscombobox.hpp"
#include "addresscombobox_p.hpp"

namespace Okteta {

AddressComboBox::AddressComboBox(QWidget* parent)
    : QWidget(parent)
    , d_ptr(new AddressComboBoxPrivate(this))
{
    Q_D(AddressComboBox);

    d->init();
}

AddressComboBox::~AddressComboBox() = default;

Address AddressComboBox::address() const
{
    Q_D(const AddressComboBox);

    return d->address();
}

int AddressComboBox::addressType() const
{
    Q_D(const AddressComboBox);

    return d->addressType();
}

int AddressComboBox::format() const
{
    Q_D(const AddressComboBox);

    return d->format();
}

void AddressComboBox::rememberCurrentAddress()
{
    Q_D(AddressComboBox);

    d->rememberCurrentAddress();
}

}

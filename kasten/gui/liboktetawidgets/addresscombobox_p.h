/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ADDRESSCOMBOBOX_P_H
#define ADDRESSCOMBOBOX_P_H

// lib
#include "addresscombobox.h"
#include "addressvalidator.h"
// KDE
#include <KComboBox>


namespace Okteta
{

class AddressComboBoxPrivate
{
  public:
    explicit AddressComboBoxPrivate( AddressComboBox* parent );

  public:
    Address address() const;
    int format() const;
    AddressValidator::AddressType addressType() const;

  public:
    void init();
    void rememberCurrentAddress();

    void onFormatChanged( int index );
    void onValueEdited( const QString& value );
    void onValueActivated( int index );

  protected:
    AddressComboBox* const q_ptr;
    Q_DECLARE_PUBLIC( AddressComboBox )

    KComboBox* mFormatComboBox;
    KComboBox* mValueComboBox;
    AddressValidator::AddressType mAddressType;
    AddressValidator* mValidator;
};


inline AddressComboBoxPrivate::AddressComboBoxPrivate( AddressComboBox* parent )
  : q_ptr( parent )
{}

inline Address AddressComboBoxPrivate::address() const
{
    return mValidator->toAddress( mValueComboBox->currentText() );
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

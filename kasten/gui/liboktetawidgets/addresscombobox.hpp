/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ADDRESSCOMBOBOX_HPP
#define KASTEN_ADDRESSCOMBOBOX_HPP

// lib
#include "oktetakastengui_export.hpp"
// Okteta core
#include <Okteta/Address>
// Qt
#include <QWidget>
// Std
#include <memory>

namespace Okteta {

class AddressComboBoxPrivate;

class OKTETAKASTENGUI_EXPORT AddressComboBox : public QWidget
{
    Q_OBJECT

public:
    // TODO: same as in AddressValidator, we need a global enum containing both value and char codings...
    // matching Okteta::ValueCoding
    // TODO: turn into flags, so the codings to be used can be defined
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding,
        OctalCoding,
        ExpressionCoding,
    };
    // TODO: same as in AddressValidator, we need a global enum
    enum AddressType
    {
        InvalidAddressType = -1,
        AbsoluteAddress = 0,
        RelativeForwards,
        RelativeBackwards
    };

public:
    explicit AddressComboBox(QWidget* parent = nullptr);
    ~AddressComboBox() override;

public: // set
    void setFormat(Coding codingId);
    void rememberCurrentAddress();

public: // get
    [[nodiscard]]
    Address address() const;
    [[nodiscard]]
    int format() const;
    [[nodiscard]]
    AddressType addressType() const;

Q_SIGNALS:
    void addressChanged(Okteta::Address address);
    void addressTypeChanged(Okteta::AddressComboBox::AddressType addressType);
    void formatChanged(int index);

protected:
    const std::unique_ptr<AddressComboBoxPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AddressComboBox)
};

}

#endif

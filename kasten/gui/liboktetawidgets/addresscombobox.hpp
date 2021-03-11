/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

class QString;
class QByteArray;

namespace Okteta {

class AddressComboBoxPrivate;

class OKTETAKASTENGUI_EXPORT AddressComboBox : public QWidget
{
    Q_OBJECT

public:
    // TODO: same as in kbytearrayvalidator.h, we need a global enum containing both value and char codings...
    // matching Okteta::ValueCoding
    // TODO: turn into flags, so the codings to be used can be defined
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        OctalCoding = 2,
        BinaryCoding = 3,
        CharCoding = 4
    };

public:
    explicit AddressComboBox(QWidget* parent = nullptr);
    ~AddressComboBox() override;

public: // set
    void rememberCurrentAddress();

public: // get
    Address address() const;
    int format() const;
    /** Is actually of type AddressValidator::AddressType */
    int addressType() const;

Q_SIGNALS:
    void addressChanged(Okteta::Address address);
    void addressTypeChanged(int newAddressType);
    void formatChanged(int index);

protected:
    const QScopedPointer<AddressComboBoxPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AddressComboBox)
};

}

#endif

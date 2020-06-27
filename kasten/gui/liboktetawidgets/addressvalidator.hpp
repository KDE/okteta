/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ADDRESSVALIDATOR_HPP
#define KASTEN_ADDRESSVALIDATOR_HPP

// Okteta core
#include <Okteta/OktetaCore>
#include <Okteta/Address>
// Qt
#include <QValidator>

namespace Okteta {

class ValueCodec;

class AddressValidator : public QValidator
{
    Q_OBJECT

public:
    // matching Okteta::ValueCoding
    enum Coding
    {
        InvalidCoding = -1,
        HexadecimalCoding = 0,
        DecimalCoding = 1,
        ExpressionCoding = 2
    };
    // XXX shouldn't this better be in address.h? Sometime later maybe
    enum AddressType
    {
        InvalidAddressType = -1,
        AbsoluteAddress = 0,
        RelativeForwards,
        RelativeBackwards
    };

public:
    explicit AddressValidator(QObject* parent, Coding codecId = HexadecimalCoding);
    ~AddressValidator() override;

public: // QValidator API
    QValidator::State validate(QString& input, int& pos) const override;

public:
    AddressType addressType() const;
    /** Sets one of the value codecs or any char codec */
    void setCodec(Coding codecId);

public:
    Address toAddress(const QString& string, AddressType* type = nullptr) const;
    QString toString(Address address, AddressType addressType) const;

private:
    Coding mCodecId;
    ValueCodec* mValueCodec;

    static const QRegExp expressionRegex;
};

}

#endif

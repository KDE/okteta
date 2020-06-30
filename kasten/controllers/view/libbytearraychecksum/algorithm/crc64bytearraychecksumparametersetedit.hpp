/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP
#define KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSETEDIT_HPP

// lib
#include "abstractbytearraychecksumparametersetedit.hpp"

class KComboBox;

class Crc64ByteArrayChecksumParameterSetEdit : public AbstractByteArrayChecksumParameterSetEdit
{
    Q_OBJECT

public:
    static const char Id[];

public:
    explicit Crc64ByteArrayChecksumParameterSetEdit(QWidget* parent = nullptr);
    ~Crc64ByteArrayChecksumParameterSetEdit() override;

public: // AbstractByteArrayFilterParameterSetEdit API
    void setParameterSet(const AbstractByteArrayChecksumParameterSet* parameterSet) override;
    void getParameterSet(AbstractByteArrayChecksumParameterSet* parameterSet) const override;
    bool isValid() const override;

private:
    KComboBox* mVariantComboBox;
};

#endif

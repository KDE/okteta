/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_QCA2BYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_QCA2BYTEARRAYCHECKSUMALGORITHM_HPP

//
#include "nobytearraychecksumparameterset.hpp"
//
#include <abstractbytearraychecksumalgorithm.hpp>

class Qca2ByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
{
    Q_OBJECT

public:
    Qca2ByteArrayChecksumAlgorithm(const QString& name, const QString& type);
    ~Qca2ByteArrayChecksumAlgorithm() override;

public: // AbstractByteArrayChecksumAlgorithm API
    bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayChecksumParameterSet* parameterSet() override;

private:
    NoByteArrayChecksumParameterSet mParameterSet;
    const QString mType;
};

#endif

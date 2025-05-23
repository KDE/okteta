/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODSUM16BYTEARRAYCHECKSUMALGORITHM_HPP
#define KASTEN_MODSUM16BYTEARRAYCHECKSUMALGORITHM_HPP

//
#include "modsumbytearraychecksumparameterset.hpp"
//
#include <abstractbytearraychecksumalgorithm.hpp>

class ModSum16ByteArrayChecksumAlgorithm : public AbstractByteArrayChecksumAlgorithm
{
    Q_OBJECT

public:
    ModSum16ByteArrayChecksumAlgorithm();
    ~ModSum16ByteArrayChecksumAlgorithm() override;

public: // AbstractByteArrayChecksumAlgorithm API
    [[nodiscard]]
    bool calculateChecksum(QString* result, const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    [[nodiscard]]
    AbstractByteArrayChecksumParameterSet* parameterSet() override;
    void loadConfig(const KConfigGroup& configGroup) override;
    void saveConfig(KConfigGroup& configGroup) const override;

private:
    [[nodiscard]]
    quint16 calculateModSumWithBigEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const;
    [[nodiscard]]
    quint16 calculateModSumWithLittleEndian(const Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const;

private:
    ModSumByteArrayChecksumParameterSet mParameterSet;
};

#endif

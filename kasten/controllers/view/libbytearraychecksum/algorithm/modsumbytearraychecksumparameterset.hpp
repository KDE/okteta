/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_MODSUMBYTEARRAYCHECKSUMPARAMETERSET_HPP

// lib
#include "abstractbytearraychecksumparameterset.hpp"
// Qt
#include <QSysInfo>

class KConfigGroup;

class ModSumByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
{
public:
    ModSumByteArrayChecksumParameterSet();
    ~ModSumByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    [[nodiscard]]
    const char* id() const override;

public:
    void setEndianness(QSysInfo::Endian endianness);

public:
    [[nodiscard]]
    QSysInfo::Endian endianness() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    QSysInfo::Endian mEndianness = QSysInfo::ByteOrder;
};

#endif

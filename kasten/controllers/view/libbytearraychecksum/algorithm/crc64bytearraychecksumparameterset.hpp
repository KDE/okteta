/*
    SPDX-FileCopyrightText: 2019 Lars Maier <lars.maier@tefax.net>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSET_HPP
#define KASTEN_CRC64BYTEARRAYCHECKSUMPARAMETERSET_HPP

// lib
#include "abstractbytearraychecksumparameterset.hpp"

class KConfigGroup;

class Crc64ByteArrayChecksumParameterSet : public AbstractByteArrayChecksumParameterSet
{
public:
    enum class Variant
    {
        ECMA182 = 0,
        ISO3309 = 1,
        _Count,
    };
    static constexpr auto VariantCount = static_cast<std::size_t>(Variant::_Count);

private:
    static constexpr Variant DefaultVariant = Variant::ECMA182;

    static constexpr char VariantConfigKey[] = "Variant";

public:
    Crc64ByteArrayChecksumParameterSet();
    ~Crc64ByteArrayChecksumParameterSet() override;

public: // AbstractByteArrayChecksumParameterSet API
    [[nodiscard]]
    const char* id() const override;

public:
    void setVariant(Variant variant);

public:
    [[nodiscard]]
    Variant variant() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    Variant mVariant = DefaultVariant;
};

#endif

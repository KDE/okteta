/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSET_HPP

// lib
#include "abstractbytearrayfilterparameterset.hpp"

class KConfigGroup;

class ReverseByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
private:
    static inline constexpr bool DefaultInvertsBits = false;

    static inline constexpr char InvertsBitsConfigKey[] = "InvertsBits";

public:
    ReverseByteArrayFilterParameterSet();
    ~ReverseByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    [[nodiscard]]
    const char* id() const override;

public:
    void setInvertsBits(bool invertsBits);

public:
    [[nodiscard]]
    bool invertsBits() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    bool mInvertsBits = false;
};

#endif

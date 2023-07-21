/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ROTATEBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_ROTATEBYTEARRAYFILTERPARAMETERSET_HPP

// lib
#include "abstractbytearrayfilterparameterset.hpp"

class KConfigGroup;

class RotateByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
private:
    static inline constexpr int DefaultGroupSize = 1;
    static inline constexpr int DefaultMoveBitWidth = 0;

    static inline constexpr char GroupSizeConfigKey[] = "GroupSize";
    static inline constexpr char MoveBitWidthConfigKey[] = "MoveBitWidth";

public:
    RotateByteArrayFilterParameterSet();
    ~RotateByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    const char* id() const override;

public:
    void setGroupSize(int groupSize);
    void setMoveBitWidth(int moveBitWidth);

public:
    int groupSize() const;
    int moveBitWidth() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    int mGroupSize = 1;
    int mMoveBitWidth = 0;
};

#endif

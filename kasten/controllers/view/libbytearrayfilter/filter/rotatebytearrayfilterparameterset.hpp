/*
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
    static constexpr int DefaultGroupSize = 1;
    static constexpr int DefaultMoveBitWidth = 0;

    static constexpr char GroupSizeConfigKey[] = "GroupSize";
    static constexpr char MoveBitWidthConfigKey[] = "MoveBitWidth";

public:
    RotateByteArrayFilterParameterSet();
    ~RotateByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    [[nodiscard]]
    const char* id() const override;

public:
    void setGroupSize(int groupSize);
    void setMoveBitWidth(int moveBitWidth);

public:
    [[nodiscard]]
    int groupSize() const;
    [[nodiscard]]
    int moveBitWidth() const;

public:
    void loadConfig(const KConfigGroup& configGroup);
    void saveConfig(KConfigGroup& configGroup) const;

private:
    int mGroupSize = DefaultGroupSize;
    int mMoveBitWidth = DefaultMoveBitWidth;
};

#endif

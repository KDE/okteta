/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "rotatebytearrayfilterparameterset.hpp"

// KF
#include <KConfigGroup>


RotateByteArrayFilterParameterSet::RotateByteArrayFilterParameterSet() = default;

RotateByteArrayFilterParameterSet::~RotateByteArrayFilterParameterSet() = default;

const char* RotateByteArrayFilterParameterSet::id() const { return "Rotate"; }

int RotateByteArrayFilterParameterSet::groupSize() const { return mGroupSize; }
int RotateByteArrayFilterParameterSet::moveBitWidth() const { return mMoveBitWidth; }

void RotateByteArrayFilterParameterSet::setGroupSize(int groupSize) { mGroupSize = groupSize; }
void RotateByteArrayFilterParameterSet::setMoveBitWidth(int moveBitWidth) { mMoveBitWidth = moveBitWidth; }

void RotateByteArrayFilterParameterSet::loadConfig(const KConfigGroup& configGroup)
{
    mGroupSize = configGroup.readEntry(GroupSizeConfigKey, DefaultGroupSize);
    mMoveBitWidth = configGroup.readEntry(MoveBitWidthConfigKey, DefaultMoveBitWidth);
}

void RotateByteArrayFilterParameterSet::saveConfig(KConfigGroup& configGroup) const
{
    configGroup.writeEntry(GroupSizeConfigKey, mGroupSize);
    configGroup.writeEntry(MoveBitWidthConfigKey, mMoveBitWidth);
}

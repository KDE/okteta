/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "reversebytearrayfilterparameterset.hpp"

ReverseByteArrayFilterParameterSet::ReverseByteArrayFilterParameterSet() = default;

ReverseByteArrayFilterParameterSet::~ReverseByteArrayFilterParameterSet() = default;

const char* ReverseByteArrayFilterParameterSet::id() const { return "Reverse"; }

bool ReverseByteArrayFilterParameterSet::invertsBits() const { return mInvertsBits; }

void ReverseByteArrayFilterParameterSet::setInvertsBits(bool invertsBits) { mInvertsBits = invertsBits; }

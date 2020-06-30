/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "nobytearrayfilterparameterset.hpp"

NoByteArrayFilterParameterSet::NoByteArrayFilterParameterSet() = default;

NoByteArrayFilterParameterSet::~NoByteArrayFilterParameterSet() = default;

const char* NoByteArrayFilterParameterSet::id() const { return "None"; }

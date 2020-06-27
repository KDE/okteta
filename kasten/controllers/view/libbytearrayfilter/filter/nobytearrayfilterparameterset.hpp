/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_NOBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_NOBYTEARRAYFILTERPARAMETERSET_HPP

// lib
#include "abstractbytearrayfilterparameterset.hpp"

class NoByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
public:
    NoByteArrayFilterParameterSet();
    ~NoByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    const char* id() const override;
};

#endif

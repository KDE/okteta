/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSET_HPP
#define KASTEN_REVERSEBYTEARRAYFILTERPARAMETERSET_HPP

// lib
#include "abstractbytearrayfilterparameterset.hpp"

class ReverseByteArrayFilterParameterSet : public AbstractByteArrayFilterParameterSet
{
public:
    ReverseByteArrayFilterParameterSet();
    ~ReverseByteArrayFilterParameterSet() override;

public: // AbstractByteArrayFilterParameterSet API
    const char* id() const override;

public:
    void setInvertsBits(bool invertsBits);

public:
    bool invertsBits() const;

private:
    bool mInvertsBits = false;
};

#endif

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ANDBYTEARRAYFILTER_HPP
#define KASTEN_ANDBYTEARRAYFILTER_HPP

// lib
#include "operandbytearrayfilterparameterset.hpp"
//
#include <abstractbytearrayfilter.hpp>

class AndByteArrayFilter : public AbstractByteArrayFilter
{
    Q_OBJECT

public:
    AndByteArrayFilter();

    ~AndByteArrayFilter() override;

public: // AbstractByteArrayFilter API
    bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayFilterParameterSet* parameterSet() override;

private:
    OperandByteArrayFilterParameterSet mParameterSet;
};

#endif

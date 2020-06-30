/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INVERTBYTEARRAYFILTER_HPP
#define KASTEN_INVERTBYTEARRAYFILTER_HPP

//
#include "nobytearrayfilterparameterset.hpp"
//
#include <abstractbytearrayfilter.hpp>

class InvertByteArrayFilter : public AbstractByteArrayFilter
{
    Q_OBJECT

public:
    InvertByteArrayFilter();

    ~InvertByteArrayFilter() override;

public: // AbstractByteArrayFilter API
    bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayFilterParameterSet* parameterSet() override;

private:
    NoByteArrayFilterParameterSet mNoParameterSet;
};

#endif

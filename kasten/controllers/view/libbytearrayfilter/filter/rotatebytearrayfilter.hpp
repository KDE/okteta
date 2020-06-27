/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ROTATEBYTEARRAYFILTER_HPP
#define KASTEN_ROTATEBYTEARRAYFILTER_HPP

//
#include "rotatebytearrayfilterparameterset.hpp"
//
#include <abstractbytearrayfilter.hpp>

class RotateByteArrayFilter : public AbstractByteArrayFilter
{
    Q_OBJECT

public:
    RotateByteArrayFilter();

    ~RotateByteArrayFilter() override;

public: // AbstractByteArrayFilter API
    bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    AbstractByteArrayFilterParameterSet* parameterSet() override;

private:
    RotateByteArrayFilterParameterSet mParameterSet;
};

#endif

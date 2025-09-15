/*
    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_XORBYTEARRAYFILTER_HPP
#define KASTEN_XORBYTEARRAYFILTER_HPP

////
#include "operandbytearrayfilterparameterset.hpp"
//
#include <abstractbytearrayfilter.hpp>

class XOrByteArrayFilter : public AbstractByteArrayFilter
{
    Q_OBJECT

public:
    XOrByteArrayFilter();

    ~XOrByteArrayFilter() override;

public: // AbstractByteArrayFilter API
    [[nodiscard]]
    bool filter(Okteta::Byte* result, Okteta::AbstractByteArrayModel* model, const Okteta::AddressRange& range) const override;
    [[nodiscard]]
    AbstractByteArrayFilterParameterSet* parameterSet() override;
    void loadConfig(const KConfigGroup& configGroup) override;
    void saveConfig(KConfigGroup& configGroup) const override;

private:
    OperandByteArrayFilterParameterSet mParameterSet;
};

#endif

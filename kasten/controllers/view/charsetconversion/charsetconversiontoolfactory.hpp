/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_CHARSETCONVERSIONTOOLFACTORY_HPP
#define KASTEN_CHARSETCONVERSIONTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT CharsetConversionToolFactory : public AbstractToolFactory
{
public:
    CharsetConversionToolFactory();
    ~CharsetConversionToolFactory() override;

public: // AbstractToolFactory API
    AbstractTool* create() const override;
};

}

#endif

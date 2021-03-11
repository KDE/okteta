/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTTOOLFACTORY_HPP
#define KASTEN_STRINGSEXTRACTTOOLFACTORY_HPP

// lib
#include "oktetakastencontrollers_export.hpp"
// Kasten core
#include <Kasten/AbstractToolFactory>

namespace Kasten {

class OKTETAKASTENCONTROLLERS_EXPORT StringsExtractToolFactory : public AbstractToolFactory
{
public:
    StringsExtractToolFactory();
    ~StringsExtractToolFactory() override;

public: // AbstractToolFactory API
    AbstractTool* create() const override;
};

}

#endif

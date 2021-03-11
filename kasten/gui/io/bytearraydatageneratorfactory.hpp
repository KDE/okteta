/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYDATAGENERATORFACTORY_HPP
#define KASTEN_BYTEARRAYDATAGENERATORFACTORY_HPP

// lib
#include "oktetakastengui_export.hpp"

template <class T> class QVector;

namespace Kasten {
class AbstractModelDataGenerator;

class OKTETAKASTENGUI_EXPORT ByteArrayDataGeneratorFactory
{
public:
    ByteArrayDataGeneratorFactory() = delete;

public:
    static QVector<AbstractModelDataGenerator*> createDataGenerators();
};

}

#endif

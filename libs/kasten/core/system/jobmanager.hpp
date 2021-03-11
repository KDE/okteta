/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_JOBMANAGER_HPP
#define KASTEN_JOBMANAGER_HPP

// lib
#include "kastencore_export.hpp"

class KJob;

namespace Kasten {

class KASTENCORE_EXPORT JobManager
{
public:
    // temporary, remove quickly
    static bool executeJob(KJob* job);

public:
};

}

#endif

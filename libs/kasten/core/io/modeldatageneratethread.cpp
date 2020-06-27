/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modeldatageneratethread.hpp"

// lib
#include "abstractmodeldatagenerator.hpp"

namespace Kasten {

ModelDataGenerateThread::~ModelDataGenerateThread() = default;

void ModelDataGenerateThread::run()
{
    mMimeData = mGenerator->generateData();

    emit generated(mMimeData);
}

}

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelstreamencodethread.hpp"

// lib
#include "abstractmodelstreamencoder.hpp"

namespace Kasten {

ModelStreamEncodeThread::~ModelStreamEncodeThread() = default;

void ModelStreamEncodeThread::run()
{
    mSuccess = mEncoder->encodeToStream(mIODevice, mModel, mSelection);

    emit modelExported(mSuccess);
}

}

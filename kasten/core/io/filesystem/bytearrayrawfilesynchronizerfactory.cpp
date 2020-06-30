/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "bytearrayrawfilesynchronizerfactory.hpp"

// lib
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QString>

namespace Kasten {

ByteArrayRawFileSynchronizerFactory::ByteArrayRawFileSynchronizerFactory() = default;

ByteArrayRawFileSynchronizerFactory::~ByteArrayRawFileSynchronizerFactory() = default;

// could be set to base class as value, is only one object per factory at runtime
// and settable in desktop file
QString ByteArrayRawFileSynchronizerFactory::supportedWorkType() const { return QStringLiteral("ByteArrayDocument");}
QString ByteArrayRawFileSynchronizerFactory::supportedRemoteType() const { return QStringLiteral("application/octet-stream");}

AbstractModelSynchronizer* ByteArrayRawFileSynchronizerFactory::createSynchronizer() const
{
    return new ByteArrayRawFileSynchronizer();
}

}

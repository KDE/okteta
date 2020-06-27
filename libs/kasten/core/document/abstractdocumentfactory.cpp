/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractdocumentfactory.hpp"

namespace Kasten {

AbstractDocumentFactory::~AbstractDocumentFactory() = default;

bool AbstractDocumentFactory::canCreateFromData(const QMimeData* mimeData)
{
    Q_UNUSED(mimeData);

    return false;
}

AbstractDocument* AbstractDocumentFactory::createFromData(const QMimeData* mimeData, bool setModified)
{
    Q_UNUSED(mimeData);
    Q_UNUSED(setModified);

    return nullptr;
}

}

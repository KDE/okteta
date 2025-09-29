/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelexporter.hpp"
#include "abstractmodelexporter_p.hpp"

// Std
#include <utility>

namespace Kasten {

AbstractModelExporter::AbstractModelExporter(std::unique_ptr<AbstractModelExporterPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractModelExporter::AbstractModelExporter(const QString& remoteTypeName, const QString& remoteMimeType)
    : d_ptr(std::make_unique<AbstractModelExporterPrivate>(this, remoteTypeName, remoteMimeType))
{
}

AbstractModelExporter::~AbstractModelExporter() = default;

QString AbstractModelExporter::remoteTypeName() const
{
    Q_D(const AbstractModelExporter);

    return d->remoteTypeName();
}

QString AbstractModelExporter::remoteMimeType() const
{
    Q_D(const AbstractModelExporter);

    return d->remoteMimeType();
}

}

#include "moc_abstractmodelexporter.cpp"

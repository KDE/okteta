/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007, 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractmodelstreamencoder.hpp"
#include "abstractmodelstreamencoder_p.hpp"

namespace Kasten {

AbstractModelStreamEncoder::AbstractModelStreamEncoder(std::unique_ptr<AbstractModelStreamEncoderPrivate>&& dd)
    : d_ptr(std::move(dd))
{
}

AbstractModelStreamEncoder::AbstractModelStreamEncoder(const QString& remoteTypeName,
                                                       const QString& remoteMimeType,
                                                       const QString& remoteClipboardMimeType)
    : d_ptr(std::make_unique<AbstractModelStreamEncoderPrivate>(this, remoteTypeName, remoteMimeType, remoteClipboardMimeType))
{
}

AbstractModelStreamEncoder::~AbstractModelStreamEncoder() = default;

QString AbstractModelStreamEncoder::remoteTypeName() const
{
    Q_D(const AbstractModelStreamEncoder);

    return d->remoteTypeName();
}

QString AbstractModelStreamEncoder::remoteMimeType() const
{
    Q_D(const AbstractModelStreamEncoder);

    return d->remoteMimeType();
}

QString AbstractModelStreamEncoder::remoteClipboardMimeType() const
{
    Q_D(const AbstractModelStreamEncoder);

    return d->remoteClipboardMimeType();
}

}

#include "moc_abstractmodelstreamencoder.cpp"

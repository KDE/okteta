/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraystreamencoder.hpp"

// lib
#include <bytearrayview.hpp>
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
#include <Kasten/Okteta/ByteArraySelection>
// Okteta core
#include <Okteta/AbstractByteArrayModel>
// Qt
#include <QBuffer>

namespace Kasten {

static constexpr int MaxPreviewSize = 100;

AbstractByteArrayStreamEncoder::AbstractByteArrayStreamEncoder(const QString& remoteTypeName,
                                                               const QString& remoteMimeType)
    : AbstractModelStreamEncoder(remoteTypeName, remoteMimeType, QStringLiteral("text/plain"))
{}

AbstractByteArrayStreamEncoder::~AbstractByteArrayStreamEncoder() = default;

QString AbstractByteArrayStreamEncoder::modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_UNUSED(selection)

    const auto * byteArrayDocument = model->findBaseModel<const ByteArrayDocument*>();

    return byteArrayDocument ? byteArrayDocument->typeName() : QString();
}

bool AbstractByteArrayStreamEncoder::encodeToStream(QIODevice* device,
                                                    AbstractModel* model, const AbstractModelSelection* selection)
{
    const auto* byteArrayView = qobject_cast<const ByteArrayView*>(model);

    const ByteArrayDocument* byteArrayDocument =
        byteArrayView ? qobject_cast<const ByteArrayDocument*>(byteArrayView->baseModel()) : nullptr;
    if (!byteArrayDocument) {
        return false;
    }

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const ByteArraySelection* byteArraySelection =
        selection ? static_cast<const ByteArraySelection*>(selection) : nullptr;

    const Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
                                       byteArraySelection->range() :
                                       Okteta::AddressRange::fromWidth(0, byteArray->size());

    const bool success = encodeDataToStream(device, byteArrayView, byteArray, range);

    return success;
}

QString AbstractByteArrayStreamEncoder::previewData(AbstractModel* model, const AbstractModelSelection* selection)
{
    const auto* byteArrayView = qobject_cast<const ByteArrayView*>(model);

    const ByteArrayDocument* byteArrayDocument =
        byteArrayView ? qobject_cast<const ByteArrayDocument*>(byteArrayView->baseModel()) : nullptr;
    if (!byteArrayDocument) {
        return {};
    }

    const Okteta::AbstractByteArrayModel* byteArray = byteArrayDocument->content();

    const ByteArraySelection* byteArraySelection =
        selection ? static_cast<const ByteArraySelection*>(selection) : nullptr;

    Okteta::AddressRange range = byteArraySelection && byteArraySelection->isValid() ?
                                 byteArraySelection->range() :
                                 Okteta::AddressRange::fromWidth(0, byteArray->size());
    range.restrictEndByWidth(MaxPreviewSize);

    QByteArray data;
    QBuffer dataBuffer(&data);
    dataBuffer.open(QIODevice::WriteOnly);

    const bool success = encodeDataToStream(&dataBuffer, byteArrayView, byteArray, range);
    dataBuffer.close();

    return success ? QString::fromLatin1(data) : QString();
}

}

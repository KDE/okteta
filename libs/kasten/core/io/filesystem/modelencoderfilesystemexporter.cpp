/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexporter.hpp"
#include "modelencoderfilesystemexporter_p.hpp"

namespace Kasten {

ModelEncoderFileSystemExporter::ModelEncoderFileSystemExporter(AbstractModelStreamEncoder* encoder)
    : AbstractModelExporter(new ModelEncoderFileSystemExporterPrivate(this, encoder->remoteTypeName(), encoder->remoteMimeType(), encoder))
{
}

ModelEncoderFileSystemExporter::~ModelEncoderFileSystemExporter() = default;

AbstractModelStreamEncoder* ModelEncoderFileSystemExporter::encoder() const
{
    Q_D(const ModelEncoderFileSystemExporter);

    return d->encoder();
}

AbstractExportJob* ModelEncoderFileSystemExporter::startExport(AbstractModel* model,
                                                               const AbstractModelSelection* selection,
                                                               const QUrl& url)
{
    Q_D(ModelEncoderFileSystemExporter);

    return d->startExport(model, selection, url);
}

QString ModelEncoderFileSystemExporter::modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const
{
    Q_D(const ModelEncoderFileSystemExporter);

    return d->modelTypeName(model, selection);
}

}

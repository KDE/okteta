/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTER_P_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTER_P_HPP

// lib
#include "modelencoderfilesystemexporter.hpp"
#include "modelencoderfilesystemexportjob.hpp"
#include <abstractmodelstreamencoder.hpp>
#include <abstractmodelexporter_p.hpp>

namespace Kasten {

class ModelEncoderFileSystemExporterPrivate : public AbstractModelExporterPrivate
{
public:
    ModelEncoderFileSystemExporterPrivate(ModelEncoderFileSystemExporter* parent,
                                          const QString& remoteTypeName, const QString& remoteMimeType,
                                          AbstractModelStreamEncoder* encoder);

    ~ModelEncoderFileSystemExporterPrivate() override;

public: // AbstractModelExporter API
    AbstractExportJob* startExport(AbstractModel* model, const AbstractModelSelection* selection,
                                   const QUrl& url);
    QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const;

public:
    AbstractModelStreamEncoder* encoder() const;

private:
    AbstractModelStreamEncoder* const mEncoder;
};

inline ModelEncoderFileSystemExporterPrivate::ModelEncoderFileSystemExporterPrivate(ModelEncoderFileSystemExporter* parent,
                                                                                    const QString& remoteTypeName, const QString& remoteMimeType,
                                                                                    AbstractModelStreamEncoder* encoder)
    : AbstractModelExporterPrivate(parent, remoteTypeName, remoteMimeType)
    , mEncoder(encoder)
{
}

inline ModelEncoderFileSystemExporterPrivate::~ModelEncoderFileSystemExporterPrivate() = default;

inline AbstractModelStreamEncoder* ModelEncoderFileSystemExporterPrivate::encoder() const { return mEncoder; }

inline AbstractExportJob* ModelEncoderFileSystemExporterPrivate::startExport(AbstractModel* model,
                                                                             const AbstractModelSelection* selection,
                                                                             const QUrl& url)
{
    return new ModelEncoderFileSystemExportJob(model, selection, url, mEncoder);
}

inline QString ModelEncoderFileSystemExporterPrivate::modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const
{
    return mEncoder->modelTypeName(model, selection);
}

}

#endif

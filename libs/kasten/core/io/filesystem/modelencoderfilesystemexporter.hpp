/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTER_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTER_HPP

// lib
#include "abstractmodelexporter.hpp"

namespace Kasten {

class AbstractModelStreamEncoder;

class ModelEncoderFileSystemExporterPrivate;

class KASTENCORE_EXPORT ModelEncoderFileSystemExporter : public AbstractModelExporter
{
    Q_OBJECT

public:
    explicit ModelEncoderFileSystemExporter(AbstractModelStreamEncoder* encoder);

    ~ModelEncoderFileSystemExporter() override;

public:
    AbstractModelStreamEncoder* encoder() const;

public: // AbstractModelExporter API
    AbstractExportJob* startExport(AbstractModel* model, const AbstractModelSelection* selection,
                                   const QUrl& url) override;
    QString modelTypeName(AbstractModel* model, const AbstractModelSelection* selection) const override;

private:
    Q_DECLARE_PRIVATE(ModelEncoderFileSystemExporter)
};

}

#endif

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "modelencoderfilesystemexportjob.hpp"
#include "modelencoderfilesystemexportjob_p.hpp"

namespace Kasten {

ModelEncoderFileSystemExportJob::ModelEncoderFileSystemExportJob(AbstractModel* model,
                                                                 const AbstractModelSelection* selection,
                                                                 const QUrl& url,
                                                                 AbstractModelStreamEncoder* encoder)
    : AbstractFileSystemExportJob(new ModelEncoderFileSystemExportJobPrivate(this, model, selection, url, encoder))
{
}

ModelEncoderFileSystemExportJob::~ModelEncoderFileSystemExportJob() = default;

void ModelEncoderFileSystemExportJob::startExportToFile()
{
    Q_D(ModelEncoderFileSystemExportJob);

    d->startExportToFile();
}

}

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODELENCODERFILESYSTEMEXPORTJOB_HPP
#define KASTEN_MODELENCODERFILESYSTEMEXPORTJOB_HPP

// lib
#include "abstractfilesystemexportjob.hpp"

namespace Kasten {

class AbstractModelStreamEncoder;

class ModelEncoderFileSystemExportJobPrivate;

class KASTENCORE_EXPORT ModelEncoderFileSystemExportJob : public AbstractFileSystemExportJob
{
    Q_OBJECT

public:
    ModelEncoderFileSystemExportJob(AbstractModel* model, const AbstractModelSelection* selection,
                                    const QUrl& url, AbstractModelStreamEncoder* encoder);
    ~ModelEncoderFileSystemExportJob() override;

protected: // AbstractFileSystemExportJob API
    void startExportToFile() override;

private:
    Q_DECLARE_PRIVATE(ModelEncoderFileSystemExportJob)
};

}

#endif

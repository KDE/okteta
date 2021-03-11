/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMEXPORTJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMEXPORTJOB_HPP

// lib
#include "abstractexportjob.hpp"

class QUrl;
class QFile;

namespace Kasten {

class AbstractModel;
class AbstractModelSelection;

class AbstractFileSystemExportJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemExportJob : public AbstractExportJob
{
    Q_OBJECT

protected:
    AbstractFileSystemExportJob(AbstractFileSystemExportJobPrivate* d);

public:
    AbstractFileSystemExportJob(AbstractModel* model, const AbstractModelSelection* selection,
                                const QUrl& url);

    ~AbstractFileSystemExportJob() override;

public: // KJob API
    void start() override;

protected: // API to be implemented
    virtual void startExportToFile() = 0;

protected:
    void completeExport(bool success);

protected:
    AbstractModel* model() const;
    const AbstractModelSelection* selection() const;
    QFile* file() const;

private:
    Q_PRIVATE_SLOT(d_func(), void exportToFile())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemExportJob)
};

}

#endif

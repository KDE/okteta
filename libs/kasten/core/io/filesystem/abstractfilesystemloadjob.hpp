/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMLOADJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMLOADJOB_HPP

// library
#include "abstractloadjob.hpp"

class QUrl;
class QFile;

namespace Kasten {

class AbstractModelFileSystemSynchronizer;

class AbstractFileSystemLoadJobPrivate;

class KASTENCORE_EXPORT AbstractFileSystemLoadJob : public AbstractLoadJob
{
    Q_OBJECT

public:
    AbstractFileSystemLoadJob(AbstractModelFileSystemSynchronizer* synchronizer, const QUrl& url);

    ~AbstractFileSystemLoadJob() override;

public: // KJob API
    void start() override;

protected: // AbstractLoadJob API
    void setDocument(AbstractDocument* document) override;

protected: // API to be implemented
    virtual void startLoadFromFile() = 0;

protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QUrl url() const;
    QFile* file() const;

private:
    Q_PRIVATE_SLOT(d_func(), void load())

private:
    Q_DECLARE_PRIVATE(AbstractFileSystemLoadJob)
};

}

#endif

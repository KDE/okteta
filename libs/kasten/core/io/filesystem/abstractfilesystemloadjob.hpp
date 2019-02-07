/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KASTEN_ABSTRACTFILESYSTEMLOADJOB_HPP
#define KASTEN_ABSTRACTFILESYSTEMLOADJOB_HPP

// library
#include <kasten/abstractloadjob.hpp>

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

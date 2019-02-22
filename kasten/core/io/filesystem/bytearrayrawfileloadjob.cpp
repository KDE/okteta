/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bytearrayrawfileloadjob.hpp"

// lib
// #include "externalbookmarkstorage.hpp"
#include "bytearrayrawfilesynchronizer.hpp"
#include "bytearrayrawfileloadthread.hpp"
#include "bytearraydocument.hpp"
// Qt
#include <QCoreApplication>
#include <QUrl>

namespace Kasten {

ByteArrayRawFileLoadJob::ByteArrayRawFileLoadJob(ByteArrayRawFileSynchronizer* synchronizer, const QUrl& url)
    : AbstractFileSystemLoadJob(synchronizer, url)
{}

ByteArrayRawFileLoadJob::~ByteArrayRawFileLoadJob() = default;

void ByteArrayRawFileLoadJob::startLoadFromFile()
{
    auto* loadThread = new ByteArrayRawFileLoadThread(this, file());
    loadThread->start();
    while (!loadThread->wait(100)) {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::ExcludeSocketNotifiers);
    }

    ByteArrayDocument* document = loadThread->document();
    qobject_cast<ByteArrayRawFileSynchronizer*>(synchronizer())->setDocument(document);

    if (document) {
//         ExternalBookmarkStorage().readBookmarks( document, url() );
    } else {
        // TODO: these reports should go to a notification system, for log or popup
        setError(KJob::KilledJobError);
        setErrorText(loadThread->errorString());
    }

    delete loadThread;

    setDocument(document);
}

}

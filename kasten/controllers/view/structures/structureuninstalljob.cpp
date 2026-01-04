/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureuninstalljob.hpp"

// tool
#include "structuresmanager.hpp"
#include <structureslogging.hpp>
// KF
#include <KIO/DeleteJob>
#include <KLocalizedString>
// Qt
#include <QUrl>

namespace Kasten {

bool StructureUninstallJob::exec()
{
    StructureDefinitionFile* const definition = m_structuresManager->definition(m_structureId);
    if (!definition) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "No such structure found to uninstall: " << m_structureId;
        return false;
    }

    const QString structureDir = definition->metaData().entryPath();

    // delete the files, the RemoveDeadEntries entry in the
    // knsrc file takes care of marking the KNS entry is as deleted
    QList<QUrl> urls;
    if (definition->location() == StructureDefinitionFile::Location::SubDir) {
        urls << QUrl::fromLocalFile(structureDir);
    } else {
        // backwarc-compat
        const QString structureBasePath = structureDir + QLatin1Char('/') + definition->metaData().id();
        urls = {
            QUrl::fromLocalFile(structureBasePath + QLatin1String(".osd")),
            QUrl::fromLocalFile(structureBasePath + QLatin1String(".desktop")),
        };
    }
    KIO::DeleteJob* const job = KIO::del(urls);
    // TODO error handling
    // TODO make async
    const bool success = job->exec();

    if (success) {
        // TODO: rather have StructuresManager listen to success signal
        m_structuresManager->reloadPaths();
    } else {
        m_errorString = job->errorString();
    }

    deleteLater();

    return success;
}

}

#include "moc_structureuninstalljob.cpp"

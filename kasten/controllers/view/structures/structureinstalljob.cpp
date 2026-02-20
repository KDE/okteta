/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureinstalljob.hpp"

// tool
#include "structuresmanager.hpp"
// KF
#include <KIO/CopyJob>
#include <KTar>
#include <KZip>
#include <KLocalizedString>
// Qt
#include <QMimeDatabase>
#include <QTemporaryFile>
// Std
#include <array>
#include <functional>

namespace Kasten {

bool StructureInstallJob::exec()
{
    if (m_structureFileUrl.isEmpty()) {
        deleteLater();
        return false;
    }

    const QString userStructuresRootDir = StructuresManager::userStructuresRootDir();

    // ensure base dir exists
    QDir(userStructuresRootDir).mkpath(QStringLiteral("."));

    QString workFilePath;
    QString desktopFileSubPath;

    QTemporaryFile tmpFile;

    // need to download first?
    if (!m_structureFileUrl.isLocalFile()) {
        // create filesystem instance of the temporary file, will be cleaned on object destruction
        const bool tmpFileSuccess = tmpFile.open();
        tmpFile.close();
        if (!tmpFileSuccess) {
            m_errorString = tmpFile.errorString();
            deleteLater();
            return false;
        }

        // TODO: support also remote dirs
        // download file into temporary file
        const QUrl tmpUrl = QUrl::fromLocalFile(tmpFile.fileName());
        KIO::CopyJob* const fileCopyJob = KIO::copy(m_structureFileUrl, tmpUrl, KIO::Overwrite);
        if (!fileCopyJob->exec()) {
            const int errorCode = fileCopyJob->error();
            const QString errorString = (errorCode != 0) ?
                fileCopyJob->errorString() :
                i18n("Unknown error (0)");

            m_errorString = i18n("Downloading failed: %1", errorString);
            deleteLater();
            return false;
        }
        workFilePath = tmpFile.fileName();
    } else {
        workFilePath = m_structureFileUrl.toLocalFile();
    }

    bool success = false;

    QDir workFilePathDir(workFilePath);
    // handle either directory or archive file selected
    if (workFilePathDir.exists()) {
        const std::array<std::function<QString()>, 2> desktopFileNameGenerators = {
            []() { return QStringLiteral("metadata.desktop"); },
            [&workFilePathDir]() { return QString(workFilePathDir.dirName() +  QLatin1String(".desktop")); },
        };
        QString desktopFileName;
        for (auto& desktopFileNameGenerator : desktopFileNameGenerators) {
            const QString triedDesktopFileName = desktopFileNameGenerator();
            if (workFilePathDir.exists(triedDesktopFileName)) {
                desktopFileName = triedDesktopFileName;
                break;
            }
        }
        // TODO. move validation checks
        if (!desktopFileName.isEmpty()) {
            KIO::CopyJob* const copyJob = KIO::copy(QUrl::fromLocalFile(workFilePath), QUrl::fromLocalFile(userStructuresRootDir));
            // TODO error handling
            // TODO make async
            success = copyJob->exec();
            if (!success) {
                m_errorString = copyJob->errorString();
            } else {
                desktopFileSubPath = workFilePathDir.dirName() + QLatin1Char('/') + desktopFileName;
            }
        } else {
            m_errorString = i18n("The structure archive is invalid.");
        }
    } else {
        const QMimeType mimeType = QMimeDatabase().mimeTypeForFile(workFilePath);

        const std::unique_ptr<KArchive> structureArchive = mimeType.inherits(QStringLiteral("application/zip")) ?
            std::unique_ptr<KArchive>(std::make_unique<KZip>(workFilePath)) :
            std::unique_ptr<KArchive>(std::make_unique<KTar>(workFilePath));

        bool archiveSuccess = structureArchive->open(QIODevice::ReadOnly);
        if (archiveSuccess) {
            const KArchiveDirectory* const structureDir = structureArchive->directory();
            const QStringList toplevelEntries = structureDir->entries();

            // support multi-structure archive
            for (const QString& entry : toplevelEntries) {
                // TODO. move validation checks
                const std::array<std::function<QString()>, 2> desktopFileNameGenerators = {
                    []() { return QStringLiteral("/metadata.desktop"); },
                    [entry]() { return entry + QLatin1String(".desktop"); },
                };
                QString desktopFileName;
                for (auto& desktopFileNameGenerator : desktopFileNameGenerators) {
                    desktopFileName = desktopFileNameGenerator();
                    archiveSuccess = (structureDir->entry(entry + QLatin1Char('/') + desktopFileName) != nullptr);
                    if (archiveSuccess) {
                        break;
                    }
                }

                if (!archiveSuccess) {
                    m_errorString = i18n("The structure archive is invalid.");
                    break;
                }

                // TODO: would overwrite existing files in existing dir
                // better query before, or check if same id and this upgrade/downgrade?
                archiveSuccess = structureDir->copyTo(userStructuresRootDir);
                if (!archiveSuccess) {
                    break;
                }
                desktopFileSubPath = entry + QLatin1Char('/') + desktopFileName;
            }
            // TODO: in case of one failure, uninstall those which did not fail again?
            structureArchive->close();
        }
        if (archiveSuccess){
            success = true;
        } else if (m_errorString.isEmpty()) {
            m_errorString = structureArchive->errorString();
        }
    }

    if (success) {
        StructureMetaData structure(userStructuresRootDir + QLatin1Char('/') + desktopFileSubPath);
        m_structureId = structure.id();
        // TODO: rather have StructuresManager listen to success signal
        m_structuresManager->reloadPaths();
    }

    deleteLater();

    return success;
}

}

#include "moc_structureinstalljob.cpp"

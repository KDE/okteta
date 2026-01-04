/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREINSTALLJOB_HPP
#define KASTEN_STRUCTUREINSTALLJOB_HPP

// KF
// #include <KJob>
// Qt
#include <QObject>
#include <QUrl>

namespace Kasten {

class StructuresManager;

class StructureInstallJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    explicit StructureInstallJob(StructuresManager* structuresManager, const QUrl& structureFileUrl);

public: // KJob API
//     virtual void start();

public:
    [[nodiscard]]
    bool exec();

    [[nodiscard]]
    QString errorString() const;

private:
    StructuresManager* const m_structuresManager;
    const QUrl m_structureFileUrl;

    QString m_errorString;
};

inline StructureInstallJob::StructureInstallJob(StructuresManager* structuresManager, const QUrl& structureFileUrl)
    : m_structuresManager(structuresManager)
    , m_structureFileUrl(structureFileUrl)
{}

inline QString StructureInstallJob::errorString() const
{
    return m_errorString;
}

}

#endif

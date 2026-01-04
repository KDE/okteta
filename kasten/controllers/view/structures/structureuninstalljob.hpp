/*
    SPDX-FileCopyrightText: 2026 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREUNINSTALLJOB_HPP
#define KASTEN_STRUCTUREUNINSTALLJOB_HPP

// KF
// #include <KJob>
// Qt
#include <QObject>
#include <QUrl>

namespace Kasten {

class StructuresManager;

class StructureUninstallJob : public QObject // not yet: KJob
{
    Q_OBJECT

public:
    explicit StructureUninstallJob(StructuresManager* structuresManager, const QString& structureId);

public: // KJob API
//     virtual void start();

public:
    [[nodiscard]]
    bool exec();

    [[nodiscard]]
    QString errorString() const;

private:
    StructuresManager* const m_structuresManager;
    const QString m_structureId;

    QString m_errorString;
};

inline StructureUninstallJob::StructureUninstallJob(StructuresManager* structuresManager, const QString& structureId)
    : m_structuresManager(structuresManager)
    , m_structureId(structureId)
{}

inline QString StructureUninstallJob::errorString() const
{
    return m_errorString;
}

}

#endif

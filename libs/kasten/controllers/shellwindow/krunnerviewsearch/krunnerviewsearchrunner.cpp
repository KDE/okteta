/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2018 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "krunnerviewsearchrunner.hpp"

// lib
#include "dbusutils_p.hpp"
#include "krunner1adaptor.hpp"
// Kasten core
#include <Kasten/DocumentManager>
#include <Kasten/AbstractDocument>
// KF5
#include <KRun>
#include <KRunner/QueryMatch>
#include <KLocalizedString>
// Qt
#include <QDBusConnection>
#include <QDebug>

namespace {
namespace ActionIds {
inline QString showView() { return QStringLiteral("showView"); }
}
}

namespace Kasten {

KRunnerViewSearchRunner::KRunnerViewSearchRunner(DocumentManager* documentManager, QObject* parent)
    : QObject(parent)
    , mDocumentManager(documentManager)
{
    new Krunner1Adaptor(this);
    qDBusRegisterMetaType<RemoteMatch>();
    qDBusRegisterMetaType<RemoteMatches>();
    qDBusRegisterMetaType<RemoteAction>();
    qDBusRegisterMetaType<RemoteActions>();
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/runner"), this);
//     QDBusConnection::sessionBus().registerService(QStringLiteral("org.kde.runners.kasten"));
}

KRunnerViewSearchRunner::~KRunnerViewSearchRunner() = default;

RemoteActions KRunnerViewSearchRunner::Actions()
{
    return {};
}

RemoteMatches KRunnerViewSearchRunner::Match(const QString& searchTerm)
{
    qDebug() << "MATCH" << searchTerm;
    RemoteMatches matches;

    const auto loadedDocuments = mDocumentManager->documents();
    for (AbstractDocument* document : loadedDocuments) {
        const int indexInTitle =  document->title().indexOf(searchTerm);
        if (indexInTitle < 0) {
            continue;
        }

        RemoteMatch m;
        m.id = document->id();
        m.text = document->title();
        m.iconName = QStringLiteral("okteta");
        m.type = Plasma::QueryMatch::ExactMatch;
        m.relevance = (indexInTitle == 0) ? 0.8 : 0.3; // TODO: which values make sense here?
        matches << m;
    }

    return matches;
}

void KRunnerViewSearchRunner::Run(const QString& id, const QString& actionId)
{
    qDebug() << "RRRUUUNN" << id << actionId;
//     if (actionId == ActionIds::showView()) {
    const auto loadedDocuments = mDocumentManager->documents();
    for (AbstractDocument* document : loadedDocuments) {
        if (document->id() == id) {
            emit mDocumentManager->focusRequested(document);
            break;
        }
    }

//     }
}

}

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "viewprofiletablemodel.hpp"

// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfileManager>
// KF
#include <KColorScheme>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QVector>
#include <QIcon>

namespace Kasten {

ViewProfileTableModel::ViewProfileTableModel(const ByteArrayViewProfileManager* viewProfileManager,
                                             QObject* parent)
    : QAbstractTableModel(parent)
    , mViewProfileManager(viewProfileManager)
{
    connect(viewProfileManager, &ByteArrayViewProfileManager::viewProfilesChanged,
            this, &ViewProfileTableModel::onViewProfilesChanged);
    connect(viewProfileManager, &ByteArrayViewProfileManager::viewProfilesRemoved,
            this, &ViewProfileTableModel::onViewProfilesChanged);
    connect(viewProfileManager, &ByteArrayViewProfileManager::defaultViewProfileChanged,
            this, &ViewProfileTableModel::onDefaultIndexChanged);
    connect(viewProfileManager, &ByteArrayViewProfileManager::viewProfilesLocked,
            this, &ViewProfileTableModel::onViewProfilesChanged);
    connect(viewProfileManager, &ByteArrayViewProfileManager::viewProfilesUnlocked,
            this, &ViewProfileTableModel::onViewProfileLocksChanged);
}

ViewProfileTableModel::~ViewProfileTableModel() = default;

int ViewProfileTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? mViewProfileManager->viewProfilesCount() : 0;
}

int ViewProfileTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant ViewProfileTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    switch (role)
    {
    case Qt::DisplayRole:
    {
        const int viewProfileIndex = index.row();

        const int tableColumn = index.column();
        switch (tableColumn)
        {
        case NameColumnId:
        {
            result = mViewProfileManager->viewProfiles().at(viewProfileIndex).viewProfileTitle();
            break;
        }
        default:
            ;
        }
        break;
    }
    case Qt::DecorationRole:
    {
        const int tableColumn = index.column();
        if (tableColumn == CurrentColumnId) {
            const int viewProfileIndex = index.row();
            const ByteArrayViewProfile::Id viewProfileId =
                mViewProfileManager->viewProfiles().at(viewProfileIndex).id();

            if (mViewProfileManager->defaultViewProfileId() == viewProfileId) {
                result = QIcon::fromTheme(QStringLiteral("starred-symbolic"));
            }
        }
        break;
    }
    case Qt::ForegroundRole:
    {
        const int viewProfileIndex = index.row();
        const ByteArrayViewProfile::Id viewProfileId =
            mViewProfileManager->viewProfiles().at(viewProfileIndex).id();

        if (mViewProfileManager->isViewProfileLocked(viewProfileId)) {
            const QPalette& palette = QApplication::palette();
            const KColorScheme colorScheme(palette.currentColorGroup(), KColorScheme::View);
            result = colorScheme.foreground(KColorScheme::InactiveText);
        }
        break;
    }
    }

    return result;
}

ByteArrayViewProfile::Id
ViewProfileTableModel::viewProfileId(const QModelIndex& index) const
{
    const int viewProfileIndex = index.row();
    const bool isValidIndex =
        (0 <= viewProfileIndex) && (viewProfileIndex < mViewProfileManager->viewProfilesCount());

    return isValidIndex ? mViewProfileManager->viewProfiles().at(viewProfileIndex).id() : ByteArrayViewProfile::Id();
}

int
ViewProfileTableModel::row(const ByteArrayViewProfile::Id& viewProfileId) const
{
    int result = -1;

    const QVector<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const int viewProfilesCount = viewProfiles.count();
    for (int i = 0; i < viewProfilesCount; ++i) {
        if (viewProfileId == viewProfiles.at(i).id()) {
            result = i;
            break;
        }
    }

    return result;
}

void ViewProfileTableModel::onDefaultIndexChanged()
{
    // simply reset the whole column, does not happen often and not worth to cache the old default
    emit dataChanged(index(CurrentColumnId, 0),
        index(CurrentColumnId, mViewProfileManager->viewProfiles().count() - 1));
}

void ViewProfileTableModel::onViewProfilesChanged()
{
    beginResetModel();
    endResetModel();
}

void ViewProfileTableModel::onViewProfileLocksChanged(const QVector<ByteArrayViewProfile::Id>& viewProfileIds)
{
    const QVector<ByteArrayViewProfile> viewProfiles = mViewProfileManager->viewProfiles();
    const int viewProfilesCount = viewProfiles.count();
    for (int i = 0; i < viewProfilesCount; ++i) {
        const ByteArrayViewProfile::Id viewProfileId = viewProfiles.at(i).id();

        if (viewProfileIds.contains(viewProfileId)) {
            emit dataChanged(index(CurrentColumnId, i),
                index(NameColumnId,    i));
        }
    }
}

}

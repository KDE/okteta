/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresselectionmodel.hpp"

// tool
#include "../structuredefinitionfile.hpp"
// KF
#include <KCategorizedSortFilterProxyModel>
#include <KLocalizedString>
// Qt
#include <QRegularExpression>
#include <QHash>
// Std
#include <algorithm>

StructuresSelectionModel::StructuresSelectionModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

StructuresSelectionModel::~StructuresSelectionModel() = default;

void StructuresSelectionModel::setStructures(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs)
{
    // KCategorizedView at least with KF 5.95 fails to handle replacement in one go
    // so split up
    beginResetModel();
    m_metaDataList.clear();
    endResetModel();

    const int firstRowInserted = 0;
    const int lastRowInserted = structureDefs.size() - 1;
    beginInsertRows(QModelIndex(), firstRowInserted, lastRowInserted);

    QHash<QString, QStringList> ids;
    ids.reserve(structureDefs.size());
    m_metaDataList.reserve(structureDefs.size());
    // consider storing structureDefs directly in the model, but needs rework to not use raw pointers
    for (const Kasten::StructureDefinitionFile* def : structureDefs) {
        const StructureMetaData metaData = def->metaData();
        m_metaDataList.append(metaData);
        ids.insert(metaData.id(), def->structureNames());
    }

    // drop no longer existing ids from the enabled list
    m_enabledList.removeStructures(ids);

    endInsertRows();
}

QStringList StructuresSelectionModel::enabledStructures() const
{
    return m_enabledList.toQStringList();
}

const StructureEnabledList& StructuresSelectionModel::enabledList() const
{
    return m_enabledList;
}

void StructuresSelectionModel::setEnabledStructures(const QStringList& enabledStructures)
{
    m_enabledList.setEnabledStructures(enabledStructures);

    // do a simple mass update for now
    const int lastRow = m_metaDataList.size()-1;
    if (lastRow >= 0) {
        const QModelIndex firstIndex = index(0, 0);
        const QModelIndex leastIndex = index(lastRow, 0);
        Q_EMIT dataChanged(firstIndex, leastIndex, {Qt::CheckStateRole});
        Q_EMIT enabledStructuresChanged();
    }
}

QVariant StructuresSelectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const int row = index.row();
    if (row >= m_metaDataList.size()) {
        return QVariant();
    }

    const StructureMetaData& metaData = m_metaDataList[row];

    switch (role) {
        case Qt::DisplayRole:
            return metaData.name();
        case CommentRole:
            return metaData.comment();
        case Qt::DecorationRole:
            return metaData.iconName();
        case Qt::CheckStateRole:
            return m_enabledList.isEnabled(metaData.id());
        case MetaDataRole:
            return QVariant::fromValue(metaData);
        case KCategorizedSortFilterProxyModel::CategoryDisplayRole:
            if (metaData.categoryId() == QLatin1String("structure/js")) {
                return i18n("Dynamic Structure Definitions");
            }
            return i18n("Structure Definitions");
        case KCategorizedSortFilterProxyModel::CategorySortRole:
            return metaData.categoryId();
        default:
            return QVariant();
    }
}

bool StructuresSelectionModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    const int row = index.row();
    if (row >= m_metaDataList.size()) {
        return false;
    }

    if (role == Qt::CheckStateRole) {
        const StructureMetaData& metaData = m_metaDataList[row];
        const QString id = metaData.id();
        const bool isEnabled = value.toBool();
        m_enabledList.setEnabled(id, isEnabled);
        Q_EMIT dataChanged(index, index, {Qt::CheckStateRole});
        Q_EMIT enabledStructuresChanged();
        return true;
    }

    return false;
}

int StructuresSelectionModel::rowCount(const QModelIndex& parent) const
{
    if (!parent.isValid()) {
        return m_metaDataList.size();
    }

    return 0;
}

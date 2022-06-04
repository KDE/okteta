/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresselectionsortfilterproxymodel.hpp"

// tool
#include "structuresselectionmodel.hpp"

StructuresSelectionSortFilterProxyModel::StructuresSelectionSortFilterProxyModel(QObject* parent)
    : KCategorizedSortFilterProxyModel(parent)
{
    m_collator.setCaseSensitivity(Qt::CaseInsensitive);

    sort(0);
}

StructuresSelectionSortFilterProxyModel::~StructuresSelectionSortFilterProxyModel() = default;

void StructuresSelectionSortFilterProxyModel::setFilterString(const QString& filterString)
{
    if (filterString == m_filterString) {
        return;
    }

    m_filterString = filterString;
    invalidate();
}

bool StructuresSelectionSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (m_filterString.isEmpty()) {
        return true;
    }

    const QModelIndex index = sourceModel()->index(sourceRow, 0);
    if (!index.isValid()) {
        return false;
    }

    const StructureMetaData metaData =
        index.data(StructuresSelectionModel::MetaDataRole).value<StructureMetaData>();

    if (metaData.name().contains(m_filterString, Qt::CaseInsensitive)) {
        return true;
    }

    return metaData.comment().contains(m_filterString, Qt::CaseInsensitive);
}

bool StructuresSelectionSortFilterProxyModel::subSortLessThan(const QModelIndex& left, const QModelIndex& right) const
{
    const QString leftName = left.data(Qt::DisplayRole).toString();
    const QString rightName = right.data(Qt::DisplayRole).toString();

    const int difference = m_collator.compare(leftName, rightName);

    return (difference < 0);
}

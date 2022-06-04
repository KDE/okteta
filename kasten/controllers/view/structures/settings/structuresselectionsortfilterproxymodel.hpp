/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTURESSELECTIONSORTFILTERPROXYMODEL_HPP
#define STRUCTURESSELECTIONSORTFILTERPROXYMODEL_HPP

// KF
#include <KCategorizedSortFilterProxyModel>
// Qt
#include <QCollator>


class StructuresSelectionSortFilterProxyModel : public KCategorizedSortFilterProxyModel
{
public:
    explicit StructuresSelectionSortFilterProxyModel(QObject* parent);
    ~StructuresSelectionSortFilterProxyModel() override;

public Q_SLOTS:
    void setFilterString(const QString& filterString);
    
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
    bool subSortLessThan(const QModelIndex& left, const QModelIndex& right) const override;

private:
    QCollator m_collator;
    QString m_filterString;
};

#endif

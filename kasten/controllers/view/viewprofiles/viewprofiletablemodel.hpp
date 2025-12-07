/*
    SPDX-FileCopyrightText: 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWPROFILETABLEMODEL_HPP
#define KASTEN_VIEWPROFILETABLEMODEL_HPP

// Okteta Gui Kasten
#include <Kasten/Okteta/ByteArrayViewProfile>
// Qt
#include <QAbstractTableModel>

template <class C> class QList;

namespace Kasten {
class ByteArrayViewProfileManager;

class ViewProfileTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        CurrentColumnId = 0,
        NameColumnId = 1,
        NoOfColumnIds = 2 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    explicit ViewProfileTableModel(const ByteArrayViewProfileManager* viewProfileManager,
                                   QObject* parent = nullptr);
    ~ViewProfileTableModel() override;

public: // QAbstractTableModel API
    [[nodiscard]]
    int rowCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent) const override;
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;

public:
    [[nodiscard]]
    ByteArrayViewProfile::Id viewProfileId(const QModelIndex& index) const;
    [[nodiscard]]
    int row(const ByteArrayViewProfile::Id& viewProfileId) const;

private Q_SLOTS:
    void onDefaultIndexChanged();
    void onViewProfilesChanged();
    void onViewProfileLocksChanged(const QList<Kasten::ByteArrayViewProfile::Id>& viewProfileIds);

private:
    const ByteArrayViewProfileManager* const mViewProfileManager;
};

}

#endif

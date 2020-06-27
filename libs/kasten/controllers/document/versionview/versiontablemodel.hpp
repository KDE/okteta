/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONTABLEMODEL_HPP
#define KASTEN_VERSIONTABLEMODEL_HPP

// Qt
#include <QAbstractTableModel>

namespace Kasten {

class DocumentVersionData;
namespace If {
class Versionable;
}
class AbstractModel;

class VersionTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        CurrentColumnId = 0,
        IdColumnId = 1,
        ChangeDescriptionColumnId = 2,
        NoOfColumnIds = 3 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    VersionTableModel(AbstractModel* model, If::Versionable* versionControl, QObject* parent = nullptr);
    ~VersionTableModel() override;

public: // QAbstractTableModel API
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public:
    void setModel(AbstractModel* model, If::Versionable* versionControl);

private Q_SLOTS:
    void onRevertedToVersionIndex(int versionIndex);
    void onHeadVersionChanged(int newHeadVersionIndex);
    void onHeadVersionDataChanged(const Kasten::DocumentVersionData& newVersionData);

private:
    AbstractModel* mModel;
    If::Versionable* mVersionControl;
    /// holds the current version index
    int mVersionIndex;
};

}

#endif

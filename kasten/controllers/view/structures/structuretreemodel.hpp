/*
    SPDX-FileCopyrightText: 2009, 2011 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURETREEMODEL_HPP
#define KASTEN_STRUCTURETREEMODEL_HPP

// Qt
#include <QAbstractItemModel>

class DataInformationBase;
class DataInformation;

namespace Kasten {
class StructuresTool;

class StructureTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles {
        DataInformationRole = Qt::UserRole,
    };

public:
    explicit StructureTreeModel(StructuresTool* tool, QObject* parent = nullptr);
    ~StructureTreeModel() override;

public: // QAbstractItemModel API
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role) const override;
    [[nodiscard]]
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]]
    QMimeData* mimeData(const QModelIndexList& indexes) const override;
    [[nodiscard]]
    QStringList mimeTypes() const override;
    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    [[nodiscard]]
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]]
    QModelIndex parent(const QModelIndex& index) const override;
    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]]
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    [[nodiscard]]
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;

private:
    [[nodiscard]]
    QModelIndex findItemInModel(DataInformationBase* data) const;

private Q_SLOTS:
    void onToolDataChange(int row, void* data);
    void onToolDataClear();
    void onChildrenAboutToBeRemoved(DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenAboutToBeInserted(DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenRemoved(const DataInformation* sender, uint startIndex, uint endIndex);
    void onChildrenInserted(const DataInformation* sender, uint startIndex, uint endIndex);

private:
    StructuresTool* const mTool;

    // just for checking in debug mode:
    DataInformation* mLastSender;
    uint mLastStartIndex;
    uint mLastEndIndex;
};

}

#endif

/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTURESSELECTIONMODEL_HPP
#define STRUCTURESSELECTIONMODEL_HPP

// tool
#include "structureenabledlist.hpp"
#include <structuremetadata.hpp>
// Qt
#include <QAbstractListModel>
#include <QVector>
#include <QMap>

namespace Kasten {
class StructureDefinitionFile;
}
class StructuresSelectionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        MetaDataRole = Qt::UserRole,
        CommentRole,
    };

public:
    explicit StructuresSelectionModel(QObject* parent);
    ~StructuresSelectionModel() override;

public:
    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]]
    bool setData(const QModelIndex& index, const QVariant &value, int role = Qt::EditRole) override;
    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

public:
    void setStructures(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs);
    void setEnabledStructures(const QStringList& enabledStructures);
    [[nodiscard]]
    QStringList enabledStructures() const;
    [[nodiscard]]
    const StructureEnabledList& enabledList() const;

Q_SIGNALS:
    void enabledStructuresChanged();

private:
    QVector<StructureMetaData> m_metaDataList;
    StructureEnabledList m_enabledList;
};

#endif

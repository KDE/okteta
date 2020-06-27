/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versiontablemodel.hpp"

// Kasten core
#include <Kasten/Versionable>
#include <Kasten/AbstractModel>
// KF
#include <KLocalizedString>

#include <QIcon>

namespace Kasten {

VersionTableModel::VersionTableModel(AbstractModel* model, If::Versionable* versionControl, QObject* parent)
    : QAbstractTableModel(parent)
    , mModel(model)
    , mVersionControl(versionControl)
    , mVersionIndex(versionControl ? versionControl->versionIndex() : 0)
{
    if (mModel) {
        connect(mModel, SIGNAL(revertedToVersionIndex(int)), SLOT(onRevertedToVersionIndex(int)));
        connect(mModel, SIGNAL(headVersionChanged(int)), SLOT(onHeadVersionChanged(int)));
        connect(mModel, SIGNAL(headVersionDataChanged(Kasten::DocumentVersionData)),
                SLOT(onHeadVersionDataChanged(Kasten::DocumentVersionData)));
    }
}

VersionTableModel::~VersionTableModel() = default;

void VersionTableModel::setModel(AbstractModel* model, If::Versionable* versionControl)
{
    if (mModel) {
        mModel->disconnect(this);
    }

    mModel = model;
    mVersionControl = versionControl;

    if (mModel) {
        connect(mModel, SIGNAL(revertedToVersionIndex(int)), SLOT(onRevertedToVersionIndex(int)));
        connect(mModel, SIGNAL(headVersionChanged(int)), SLOT(onHeadVersionChanged(int)));
        connect(mModel, SIGNAL(headVersionDataChanged(Kasten::DocumentVersionData)),
                SLOT(onHeadVersionDataChanged(Kasten::DocumentVersionData)));
    }
    mVersionIndex = versionControl ? versionControl->versionIndex() : 0;

    beginResetModel();
    endResetModel();
}

int VersionTableModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid() && mVersionControl) ? mVersionControl->versionCount() : 0;
}

int VersionTableModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant VersionTableModel::data(const QModelIndex& index, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole) {
        const int versionIndex = index.row();
        const DocumentVersionData version = mVersionControl->versionData(versionIndex);

        const int tableColumn = index.column();
        switch (tableColumn)
        {
        case IdColumnId:
        {
            result = version.id();
            break;
        }
        case ChangeDescriptionColumnId:
            result = version.changeComment();
            break;
        default:
            ;
        }
    } else if (role == Qt::DecorationRole) {
        const int tableColumn = index.column();
        if (tableColumn == CurrentColumnId) {
            const int versionIndex = index.row();
            if (mVersionControl->versionIndex() == versionIndex) {
                result = QIcon::fromTheme(QStringLiteral("arrow-right"));
            }
        }
    }

    return result;
}

QVariant VersionTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
            section == IdColumnId ?                i18nc("@title:column Id of the version",         "Id") :
            section == ChangeDescriptionColumnId ? i18nc("@title:column description of the change", "Changes") :
            QString();
        result = titel;
    } else if (role == Qt::ToolTipRole) {
        const QString titel =
            section == IdColumnId ?                i18nc("@info:tooltip", "Id of the version") :
            section == ChangeDescriptionColumnId ? i18nc("@info:tooltip", "Description of what changed") :
            QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

void VersionTableModel::onRevertedToVersionIndex(int versionIndex)
{
    if (mVersionIndex == versionIndex) {
        return;
    }

    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged(index(versionIndex, CurrentColumnId), index(versionIndex, CurrentColumnId));
    emit dataChanged(index(oldVersionIndex, CurrentColumnId), index(oldVersionIndex, CurrentColumnId));
}

void VersionTableModel::onHeadVersionChanged(int newHeadVersionIndex)
{
    mVersionIndex = newHeadVersionIndex;
    // TODO: try to understand how this would be done with {begin,end}{Insert,Remove}Columns
    beginResetModel();
    endResetModel();
}

void VersionTableModel::onHeadVersionDataChanged(const DocumentVersionData& versionData)
{
    Q_UNUSED(versionData)
    const int headVersionIndex = mVersionControl->versionCount() - 1;
    emit dataChanged(index(headVersionIndex, CurrentColumnId), index(headVersionIndex, ChangeDescriptionColumnId));
}

}

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentlistmodel.hpp"

// lib
#include "documentstool.hpp"
// Kasten core
#include <Kasten/AbstractModelSynchronizer>
#include <Kasten/AbstractDocument>
// KF
#include <KLocalizedString>

#include <QIcon>

namespace Kasten {

DocumentListModel::DocumentListModel(DocumentsTool* documentsTool, QObject* parent)
    : QAbstractTableModel(parent)
    , mDocumentsTool(documentsTool)
{
    connect(mDocumentsTool, &DocumentsTool::documentsAdded,
            this, &DocumentListModel::onDocumentsAdded);
    connect(mDocumentsTool, &DocumentsTool::documentsClosing,
            this, &DocumentListModel::onDocumentsClosing);
    connect(mDocumentsTool, &DocumentsTool::focussedDocumentChanged,
            this, &DocumentListModel::onFocussedDocumentChanged);
}

DocumentListModel::~DocumentListModel() = default;

int DocumentListModel::rowCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? mDocumentsTool->documents().size() : 0;
}

int DocumentListModel::columnCount(const QModelIndex& parent) const
{
    return (!parent.isValid()) ? NoOfColumnIds : 0;
}

QVariant DocumentListModel::data(const QModelIndex& index, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const int documentIndex = index.row();
        const AbstractDocument* document = mDocumentsTool->documents().at(documentIndex);

        const int tableColumn = index.column();
        switch (tableColumn)
        {
        case TitleColumnId:
            result = document->title();
            break;
        default:
            ;
        }
    } else if (role == Qt::DecorationRole) {
        const int documentIndex = index.row();
        const AbstractDocument* document = mDocumentsTool->documents().at(documentIndex);
        const AbstractModelSynchronizer* synchronizer = document ? document->synchronizer() : nullptr;

        const int tableColumn = index.column();
        switch (tableColumn)
        {
        case CurrentColumnId:
            if (document == mDocumentsTool->focussedDocument()) {
                result = QIcon::fromTheme(QStringLiteral("arrow-right"));
            }
            break;
        case LocalStateColumnId:
            if (synchronizer && synchronizer->localSyncState() == LocalHasChanges) {
                result = QIcon::fromTheme(QStringLiteral("document-save"));
            }
            break;
        case RemoteStateColumnId:
            // TODO: use static map, syncState int -> iconname
            if (!synchronizer) {
                result = QIcon::fromTheme(QStringLiteral("document-new"));
            } else {
                const RemoteSyncState remoteSyncState = synchronizer->remoteSyncState();
                if (remoteSyncState == RemoteHasChanges) {
                    result = QIcon::fromTheme(QStringLiteral("document-save"));
                } else if (remoteSyncState == RemoteDeleted) {
                    result = QIcon::fromTheme(QStringLiteral("edit-delete"));
                } else if (remoteSyncState == RemoteUnknown) {
                    result = QIcon::fromTheme(QStringLiteral("flag-yellow"));
                } else if (remoteSyncState == RemoteUnreachable) {
                    result = QIcon::fromTheme(QStringLiteral("network-disconnect"));
                }
            }
            break;
        default:
            ;
        }
    }

    return result;
}

QVariant DocumentListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;

    if (role == Qt::DisplayRole) {
        const QString titel =
//             section == LocalStateColumnId ?  i18nc("@title:column Id of the version",         "Id") :
            section == TitleColumnId ?     i18nc("@title:column description of the change", "Title") :
            QString();
        result = titel;
    } else if (role == Qt::ToolTipRole) {
        const QString titel =
//             section == LocalStateColumnId ?                i18nc("@info:tooltip","Id of the version") :
            section == TitleColumnId ? i18nc("@info:tooltip", "Title of the document") :
            QString();
        result = titel;
    } else {
        result = QAbstractTableModel::headerData(section, orientation, role);
    }

    return result;
}

void DocumentListModel::onFocussedDocumentChanged(AbstractDocument* document)
{
    Q_UNUSED(document)

    beginResetModel();
    endResetModel();
// TODO: store current focused document, then only emit for changed
#if 0
    const int oldVersionIndex = mVersionIndex;
    mVersionIndex = versionIndex;

    emit dataChanged(index(versionIndex, CurrentColumnId), index(versionIndex, CurrentColumnId));
    emit dataChanged(index(oldVersionIndex, CurrentColumnId), index(oldVersionIndex, CurrentColumnId));
#endif
}

void DocumentListModel::onDocumentsAdded(const QVector<Kasten::AbstractDocument*>& documents)
{
    for (AbstractDocument* document : documents) {
        connect(document, &AbstractDocument::synchronizerChanged,
                this, &DocumentListModel::onSynchronizerChanged);
        AbstractModelSynchronizer* synchronizer = document->synchronizer();
        if (synchronizer) {
            connect(synchronizer, &AbstractModelSynchronizer::localSyncStateChanged,
                    this, &DocumentListModel::onSyncStatesChanged);
            connect(synchronizer, &AbstractModelSynchronizer::remoteSyncStateChanged,
                    this, &DocumentListModel::onSyncStatesChanged);
        }
    }

    // TODO: try to understand how this would be done with {begin,end}{Insert,Remove}Columns
    beginResetModel();
    endResetModel();
}

void DocumentListModel::onDocumentsClosing(const QVector<Kasten::AbstractDocument*>& documents)
{
    Q_UNUSED(documents)
    // TODO: try to understand how this would be done with {begin,end}{Insert,Remove}Columns
    beginResetModel();
    endResetModel();
}

void DocumentListModel::onSynchronizerChanged(AbstractModelSynchronizer* synchronizer)
{
    // TODO: what about the old synchronizer? assume it is deleted and that way disconnects?
    if (synchronizer) {
        connect(synchronizer, &AbstractModelSynchronizer::localSyncStateChanged,
                this, &DocumentListModel::onSyncStatesChanged);
        connect(synchronizer, &AbstractModelSynchronizer::remoteSyncStateChanged,
                this, &DocumentListModel::onSyncStatesChanged);
    }
    // TODO: try to understand how this would be done with {begin,end}{Insert,Remove}Columns
    beginResetModel();
    endResetModel();
}

void DocumentListModel::onSyncStatesChanged()
{
    // TODO: try to understand how this would be done with {begin,end}{Insert,Remove}Columns
    beginResetModel();
    endResetModel();
}

}

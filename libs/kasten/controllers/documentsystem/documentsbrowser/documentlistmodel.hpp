/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTLISTMODEL_HPP
#define KASTEN_DOCUMENTLISTMODEL_HPP

// Qt
#include <QAbstractTableModel>

namespace Kasten {

class DocumentsTool;
class AbstractModelSynchronizer;
class AbstractDocument;

class DocumentListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnIds
    {
        CurrentColumnId = 0,
        LocalStateColumnId = 1,
        RemoteStateColumnId = 2,
        TitleColumnId = 3,
        NoOfColumnIds = 4 // TODO: what pattern is usually used to mark number of ids?
    };

public:
    explicit DocumentListModel(DocumentsTool* documentsTool, QObject* parent = nullptr);
    ~DocumentListModel() override;

public: // QAbstractTableModel API
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private Q_SLOTS:
    void onDocumentsAdded(const QVector<Kasten::AbstractDocument*>& documents);
    void onDocumentsClosing(const QVector<Kasten::AbstractDocument*>& documents);
    void onFocussedDocumentChanged(Kasten::AbstractDocument* document);
    void onSyncStatesChanged();
    void onSynchronizerChanged(Kasten::AbstractModelSynchronizer* synchronizer);

private:
    DocumentsTool* mDocumentsTool;
};

}

#endif

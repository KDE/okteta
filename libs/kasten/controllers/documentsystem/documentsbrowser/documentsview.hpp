/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTSVIEW_HPP
#define KASTEN_DOCUMENTSVIEW_HPP

// Qt
#include <QWidget>

class QTreeView;
class QModelIndex;

namespace Kasten {

class DocumentListModel;
class DocumentsTool;

class DocumentsView : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentsView(DocumentsTool* tool, QWidget* parent = nullptr);
    ~DocumentsView() override;

public:
    DocumentsTool* tool() const;

private Q_SLOTS:
    void onDocumentActivated(const QModelIndex& index);

private:
    DocumentsTool* mTool;

    DocumentListModel* mDocumentListModel;

    QTreeView* mDocumentListView;
};

inline DocumentsTool* DocumentsView::tool() const { return mTool; }

}

#endif

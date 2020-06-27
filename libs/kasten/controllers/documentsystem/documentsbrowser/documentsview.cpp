/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentsview.hpp"

// tool
#include "documentlistmodel.hpp"
#include "documentstool.hpp"
// Qt
#include <QTreeView>
#include <QLayout>

namespace Kasten {

DocumentsView::DocumentsView(DocumentsTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    mDocumentListModel = new DocumentListModel(mTool, this);

    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    mDocumentListView = new QTreeView(this);
    mDocumentListView->setObjectName(QStringLiteral("DocumentListView"));
    mDocumentListView->setRootIsDecorated(false);
    mDocumentListView->setItemsExpandable(false);
    mDocumentListView->setUniformRowHeights(true);
    mDocumentListView->setAllColumnsShowFocus(true);
    mDocumentListView->setModel(mDocumentListModel);
    connect(mDocumentListView, &QAbstractItemView::activated,
            this, &DocumentsView::onDocumentActivated);
    for (int c = 0; c < DocumentListModel::NoOfColumnIds; ++c) {
        mDocumentListView->resizeColumnToContents(c);
    }

    baseLayout->addWidget(mDocumentListView, 10);
}

DocumentsView::~DocumentsView() = default;

void DocumentsView::onDocumentActivated(const QModelIndex& index)
{
    const int documentIndex = index.row();
    AbstractDocument* document = mTool->documents().at(documentIndex);

    if (document) {
        mTool->setFocussedDocument(document);
    }
}

}

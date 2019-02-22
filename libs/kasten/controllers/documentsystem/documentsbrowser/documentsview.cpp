/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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
    baseLayout->setMargin(0);
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

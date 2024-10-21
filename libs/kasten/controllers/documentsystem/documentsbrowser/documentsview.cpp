/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentsview.hpp"

// tool
#include "documentlistmodel.hpp"
#include "documentstool.hpp"
// Kasten core
#include <Kasten/AbstractDocument>
// KF
#include <KLocalizedString>
// Qt
#include <QTreeView>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

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

    // TODO. share code for all these empty-list placeholders
    auto* documentListViewViewPort = mDocumentListView->viewport();
    m_emptyListOverlayLabel = new QLabel(documentListViewViewPort);
    m_emptyListOverlayLabel->setText(i18nc("@info", "No documents"));
    m_emptyListOverlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_emptyListOverlayLabel->setEnabled(false);
    m_emptyListOverlayLabel->setWordWrap(true);
    m_emptyListOverlayLabel->setAlignment(Qt::AlignCenter);
    m_emptyListOverlayLabel->setVisible(mTool->isDocumentListEmpty());
    auto* centeringLayout = new QVBoxLayout(documentListViewViewPort);
    centeringLayout->addWidget(m_emptyListOverlayLabel);
    centeringLayout->setAlignment(m_emptyListOverlayLabel, Qt::AlignCenter);
    connect(mTool, &DocumentsTool::documentsAdded,
            this, &DocumentsView::updateEmptyListOverlayLabel);
    connect(mTool, &DocumentsTool::documentsClosing,
            this, &DocumentsView::updateEmptyListOverlayLabel);

    baseLayout->addWidget(mDocumentListView, 10);
}

DocumentsView::~DocumentsView() = default;

void DocumentsView::updateEmptyListOverlayLabel()
{
    m_emptyListOverlayLabel->setVisible(mTool->isDocumentListEmpty());
}


void DocumentsView::onDocumentActivated(const QModelIndex& index)
{
    const int documentIndex = index.row();
    AbstractDocument* document = mTool->documents().at(documentIndex);

    if (document) {
        mTool->setFocussedDocument(document);
    }
}

}

#include "moc_documentsview.cpp"

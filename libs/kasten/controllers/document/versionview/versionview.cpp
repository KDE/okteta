/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "versionview.hpp"

// tool
#include "versiontablemodel.hpp"
#include "versionviewtool.hpp"
// Kasten core
#include <Kasten/Versionable>
#include <Kasten/AbstractModel>
// Qt
#include <QLayout>
#include <QTreeView>

namespace Kasten {

VersionView::VersionView(VersionViewTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    mVersionTableModel = new VersionTableModel(nullptr, nullptr, this);

    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    mVersionTableView = new QTreeView(this);
    mVersionTableView->setObjectName(QStringLiteral("VersionsTable"));
    mVersionTableView->setRootIsDecorated(false);
    mVersionTableView->setItemsExpandable(false);
    mVersionTableView->setUniformRowHeights(true);
    mVersionTableView->setAllColumnsShowFocus(true);
    mVersionTableView->setModel(mVersionTableModel);

    baseLayout->addWidget(mVersionTableView, 10);

    connect(mTool, &Kasten::VersionViewTool::modelChanged,
            this, &VersionView::setModel);

    setModel(mTool->model());
}

VersionView::~VersionView() = default;

void VersionView::setModel(AbstractModel* model)
{
    If::Versionable* versionControl = model ? qobject_cast<If::Versionable*>(model) : nullptr;

    mVersionTableModel->setModel(model, versionControl);

    for (int c = 0; c < VersionTableModel::NoOfColumnIds; ++c) {
        mVersionTableView->resizeColumnToContents(c);
    }
}

}

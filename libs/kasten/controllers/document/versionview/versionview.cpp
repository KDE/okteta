/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

    QVBoxLayout* baseLayout = new QVBoxLayout(this);
    baseLayout->setMargin(0);

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

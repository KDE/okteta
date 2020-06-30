/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptloggerview.hpp"

#include <KComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>

#include "scriptlogger.hpp"

ScriptLoggerView::ScriptLoggerView(const TopLevelDataInformation::List& data, QWidget* parent)
    : QWidget(parent)
    , mSelector(new KComboBox(this))
    , mView(new QTableView(this))
    , mList(data)
{
    for (const auto& info : qAsConst(mList)) {
        mSelector->addItem(info->objectName());
    }

    mView->setShowGrid(false);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setWordWrap(false);
    QHeaderView* horizHeader = mView->horizontalHeader();
    horizHeader->setAcceptDrops(false);
    horizHeader->setSectionResizeMode(QHeaderView::Interactive);
    horizHeader->setSortIndicatorShown(false);
    horizHeader->setStretchLastSection(true);
    if (!mList.isEmpty()) {
        mView->setModel(mList.at(0)->logger());
        mView->resizeRowsToContents();
    }
    connect(mSelector, QOverload<int>::of(&KComboBox::currentIndexChanged),
            this, &ScriptLoggerView::updateModel);
    auto* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(mSelector, 0);
    layout->addWidget(mView, 1);
    setLayout(layout);
}

ScriptLoggerView::~ScriptLoggerView() = default;

void ScriptLoggerView::updateModel(int index)
{
    Q_ASSERT(index >= 0 && index < mList.size());
    mView->setModel(mList.at(index)->logger());
    mView->resizeRowsToContents();
}

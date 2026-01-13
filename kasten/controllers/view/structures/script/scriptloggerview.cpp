/*
    SPDX-FileCopyrightText: 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "scriptloggerview.hpp"

// lib
#include "scriptlogger.hpp"
#include <datainformation.hpp>
// KF
#include <KComboBox>
// Qt
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
// Std
#include <utility>

ScriptLoggerView::ScriptLoggerView(const QVector<std::shared_ptr<TopLevelDataInformation>>& topDataInfoList, QWidget* parent)
    : QWidget(parent)
    , mSelector(new KComboBox(this))
    , mView(new QTableView(this))
    , mList(topDataInfoList)
{
    for (const auto& info : std::as_const(mList)) {
        mSelector->addItem(info->actualDataInformation()->name());
    }

    mView->setShowGrid(false);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    mView->setWordWrap(false);
    QHeaderView* const horizHeader = mView->horizontalHeader();
    horizHeader->setAcceptDrops(false);
    horizHeader->setSectionResizeMode(QHeaderView::Interactive);
    horizHeader->setSortIndicatorShown(false);
    horizHeader->setStretchLastSection(true);
    if (!mList.isEmpty()) {
        mView->setModel(mList.at(0)->logger());
        mView->resizeRowsToContents();
    }
    connect(mSelector, qOverload<int>(&KComboBox::currentIndexChanged),
            this, &ScriptLoggerView::updateModel);
    auto* const layout = new QVBoxLayout();
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

#include "moc_scriptloggerview.cpp"

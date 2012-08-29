/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2012 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "scriptloggerview.h"

#include <KComboBox>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>

#include "scriptlogger.h"

ScriptLoggerView::ScriptLoggerView(const TopLevelDataInformation::List& data, QWidget* parent)
        : QWidget(parent), mSelector(new KComboBox(this)), mView(new QTableView(this)), mList(data)
{
    for (int i = 0; i < mList.size(); ++i)
    {
        mSelector->addItem(mList.at(i)->objectName());
    }
    mView->setShowGrid(false);
    mView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QHeaderView* horizHeader = mView->horizontalHeader();
    horizHeader->setAcceptDrops(false);
    horizHeader->setResizeMode(QHeaderView::Interactive);
    horizHeader->setSortIndicatorShown(false);
    horizHeader->setStretchLastSection(true);
    if (!mList.isEmpty())
    {
        mView->setModel(mList.at(0)->logger());
        mView->resizeRowsToContents();
    }
    connect(mSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModel(int)));
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(mSelector, 0);
    layout->addWidget(mView, 1);
    setLayout(layout);
}

ScriptLoggerView::~ScriptLoggerView()
{
}

void ScriptLoggerView::updateModel(int index)
{
    Q_ASSERT(index >= 0 && index < mList.size());
    mView->setModel(mList.at(index)->logger());
    mView->resizeRowsToContents();
}

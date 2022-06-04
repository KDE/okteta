/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresselector.hpp"

// tool
#include "structureitemdelegate.hpp"
#include "structuresselectionmodel.hpp"
#include "structuresselectionsortfilterproxymodel.hpp"
// KF
#include <KCategorizedView>
#include <KCategoryDrawer>
#include <KLocalizedString>
// Qt
#include <QLineEdit>
#include <QVBoxLayout>

StructuresSelector::StructuresSelector(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // data models
    m_structuresModel = new StructuresSelectionModel(this);
    connect(m_structuresModel, &StructuresSelectionModel::enabledStructuresChanged,
            this, &StructuresSelector::enabledStructuresChanged);
    m_soirtFilterProxyModel = new StructuresSelectionSortFilterProxyModel(this);
    m_soirtFilterProxyModel->setCategorizedModel(true);
    m_soirtFilterProxyModel->setSourceModel(m_structuresModel);

    // search field
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setClearButtonEnabled(true);
    m_lineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search..."));
    connect(m_lineEdit, &QLineEdit::textChanged,
            m_soirtFilterProxyModel, &StructuresSelectionSortFilterProxyModel::setFilterString);

    layout->addWidget(m_lineEdit);

    // list view
    m_listView = new KCategorizedView(this);
    m_categoryDrawer = new KCategoryDrawer(m_listView);
    m_listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listView->setAlternatingRowColors(true);
    m_listView->setCategoryDrawer(m_categoryDrawer);
    m_listView->setModel(m_soirtFilterProxyModel);
    m_listView->setAlternatingRowColors(true);

    m_structureDelegate = new StructureItemDelegate(m_listView, this);
    m_listView->setItemDelegate(m_structureDelegate);

    m_listView->setMouseTracking(true);
    m_listView->viewport()->setAttribute(Qt::WA_Hover);

    layout->addWidget(m_listView);

    setFocusProxy(m_lineEdit);
}

StructuresSelector::~StructuresSelector()
{
    // assist with clean destruction
    delete m_structureDelegate;
    delete m_listView;
}

void StructuresSelector::setStructures(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs)
{
    m_structuresModel->setStructures(structureDefs);
    m_soirtFilterProxyModel->sort(0);
}

void StructuresSelector::setEnabledStructures(const QStringList& enabledStructures)
{
    m_structuresModel->setEnabledStructures(enabledStructures);
}

QStringList StructuresSelector::enabledStructures() const
{
    return m_structuresModel->enabledStructures();
}

const StructureEnabledList& StructuresSelector::enabledList() const
{
    return m_structuresModel->enabledList();
}

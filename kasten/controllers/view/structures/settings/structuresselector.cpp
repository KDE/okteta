/*
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
#include <QDragEnterEvent>
#include <QDragMoveEvent>

namespace Kasten {

StructuresSelector::StructuresSelector(QWidget* parent)
    : QWidget(parent)
{
    setAcceptDrops(true);

    auto* const layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // data models
    m_structuresModel = new StructuresSelectionModel(this);
    connect(m_structuresModel, &StructuresSelectionModel::enabledStructuresChanged,
            this, &StructuresSelector::enabledStructuresChanged);
    m_sortFilterProxyModel = new StructuresSelectionSortFilterProxyModel(this);
    m_sortFilterProxyModel->setCategorizedModel(true);
    m_sortFilterProxyModel->setSourceModel(m_structuresModel);

    // search field
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setClearButtonEnabled(true);
    m_lineEdit->setPlaceholderText(i18nc("@info:placeholder", "Search…"));
    connect(m_lineEdit, &QLineEdit::textChanged,
            m_sortFilterProxyModel, &StructuresSelectionSortFilterProxyModel::setFilterString);

    layout->addWidget(m_lineEdit);

    // list view
    m_listView = new KCategorizedView(this);
    m_categoryDrawer = new KCategoryDrawer(m_listView);
    m_listView->setVerticalScrollMode(QListView::ScrollPerPixel);
    m_listView->setAlternatingRowColors(true);
    m_listView->setCategoryDrawer(m_categoryDrawer);
    m_listView->setModel(m_sortFilterProxyModel);
    m_listView->setAlternatingRowColors(true);

    m_structureDelegate = new StructureItemDelegate(m_listView, this);
    connect(m_structureDelegate, &StructureItemDelegate::uninstallStructureRequested,
            this, &StructuresSelector::onUninstallStructureRequested);
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

void StructuresSelector::setStructures(const std::map<QString, std::unique_ptr<StructureDefinitionFile>>& structureDefs)
{
    m_structuresModel->setStructures(structureDefs);
    m_sortFilterProxyModel->sort(0);
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

void StructuresSelector::onUninstallStructureRequested(const QModelIndex& index)
{
    const QString id = index.data(StructuresSelectionModel::IdRole).toString();

    if (id.isEmpty()) {
        return;
    }

    Q_EMIT uninstallStructureRequested(id);
}

void StructuresSelector::dragEnterEvent(QDragEnterEvent* event)
{
    bool isAccepted = false;
    const QMimeData* const mimeData = event->mimeData();

    Q_EMIT dataOffered(mimeData, isAccepted);

    if (isAccepted) {
        event->setAccepted(true);
        return;
    }

    QWidget::dragEnterEvent(event);
}

void StructuresSelector::dragMoveEvent(QDragMoveEvent* event)
{
    bool isAccepted = false;
    const QMimeData* const mimeData = event->mimeData();

    Q_EMIT dataOffered(mimeData, isAccepted);

    if (isAccepted) {
        event->setAccepted(true);
        return;
    }

    QWidget::dragMoveEvent(event);
}

void StructuresSelector::dropEvent(QDropEvent* event)
{
    const QMimeData* const mimeData = event->mimeData();

    Q_EMIT dataDropped(mimeData);
}

}

#include "moc_structuresselector.cpp"

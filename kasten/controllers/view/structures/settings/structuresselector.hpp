/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTURESSELECTOR_HPP
#define KASTEN_STRUCTURESSELECTOR_HPP

// tool
#include <structuremetadata.hpp>
// Qt
#include <QWidget>
// Std
#include <map>
#include <memory>

class KCategorizedView;
class KCategoryDrawer;

class QLineEdit;
class QModelIndex;
class QMimeData;

namespace Kasten {

class StructureDefinitionFile;
class StructuresSelectionSortFilterProxyModel;
class StructuresSelectionModel;
class StructureItemDelegate;
class StructureEnabledList;

class StructuresSelector : public QWidget
{
    Q_OBJECT

public:
    explicit StructuresSelector(QWidget* parent = nullptr);

    ~StructuresSelector() override;

public:
    void setStructures(const std::map<QString, std::unique_ptr<StructureDefinitionFile>>& structureDefs);
    void setEnabledStructures(const QStringList& enabledStructures);

    [[nodiscard]]
    QStringList enabledStructures() const;
    [[nodiscard]]
    const StructureEnabledList& enabledList() const;

Q_SIGNALS:
    void enabledStructuresChanged();
    void uninstallStructureRequested(const QString& id);
    void dataOffered(const QMimeData* mimeData, bool& isAccepted);
    void dataDropped(const QMimeData* mimeData);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;

private Q_SLOTS:
    void onUninstallStructureRequested(const QModelIndex& index);

private:
    StructuresSelectionModel* m_structuresModel;
    StructuresSelectionSortFilterProxyModel* m_sortFilterProxyModel;

    QLineEdit* m_lineEdit;
    KCategorizedView* m_listView;
    KCategoryDrawer* m_categoryDrawer;
    StructureItemDelegate* m_structureDelegate;
};

}

#endif

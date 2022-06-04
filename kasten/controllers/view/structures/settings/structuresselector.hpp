/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTURESSELECTOR_HPP
#define STRUCTURESSELECTOR_HPP

// tool
#include "../structuremetadata.hpp"
// Qt
#include <QWidget>
#include <QMap>

class KCategorizedView;
class KCategoryDrawer;

class QLineEdit;

namespace Kasten {
class StructureDefinitionFile;
}
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
    void setStructures(const QMap<QString, Kasten::StructureDefinitionFile*>& structureDefs);
    void setEnabledStructures(const QStringList& enabledStructures);

    QStringList enabledStructures() const;
    const StructureEnabledList& enabledList() const;

Q_SIGNALS:
    void enabledStructuresChanged();

private:
    StructuresSelectionModel* m_structuresModel;
    StructuresSelectionSortFilterProxyModel* m_soirtFilterProxyModel;

    QLineEdit* m_lineEdit;
    KCategorizedView* m_listView;
    KCategoryDrawer* m_categoryDrawer;
    StructureItemDelegate* m_structureDelegate;
};

#endif

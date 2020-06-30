/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

 SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KASTEN_STRUCTURESMANAGERVIEW_HPP
#define KASTEN_STRUCTURESMANAGERVIEW_HPP

// KF
#include <KNS3/Entry>
// Qt
#include <QWidget>

class StructureAddRemoveWidget;
class QPushButton;
class KPluginSelector;

namespace KNS3 {
class Button;
}
namespace Kasten {
class StructuresManager;
class StructuresTool;
}

class StructuresManagerView : public QWidget
{
    Q_OBJECT

public:
    Q_PROPERTY(QStringList values READ values NOTIFY changed USER true)
    explicit StructuresManagerView(Kasten::StructuresTool* manager, QWidget* parent = nullptr);

    ~StructuresManagerView() override;

public:
    QStringList values() const;

Q_SIGNALS:
    void selectedPluginsChanged();
    void changed(const QStringList& newValues);

private Q_SLOTS:
    void onGetNewStructuresClicked(const KNS3::Entry::List& changedEntries);
    void onPluginSelectorChange(bool change);
    void advancedSelection();
    void setSelectedStructures(const QStringList& selectedStructures);

private:
    void rebuildPluginSelectorEntries();
    void reloadSelectedItems();

private:
    Kasten::StructuresTool* mTool;
    QStringList mSelectedStructures;
    KNS3::Button* mGetNewStructuresButton;
    QPushButton* mAdvancedSelectionButton;
    KPluginSelector* mStructuresSelector = nullptr;
    bool mRebuildingPluginsList : 1;

};

#endif

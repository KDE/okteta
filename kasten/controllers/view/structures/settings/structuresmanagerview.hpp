/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 Copyright 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

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

#ifndef STRUCTURESMANAGERVIEW_HPP
#define STRUCTURESMANAGERVIEW_HPP

// KF5
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

protected Q_SLOTS:
    void onGetNewStructuresClicked(const KNS3::Entry::List& changedEntries);
    void onPluginSelectorChange(bool change);
    void advancedSelection();
Q_SIGNALS:
    void selectedPluginsChanged();
    void changed(const QStringList& newValues);

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
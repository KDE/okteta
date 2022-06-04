/*
 This file is part of the Okteta Kasten Framework, made within the KDE community.

 SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

 SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef KASTEN_STRUCTURESMANAGERVIEW_HPP
#define KASTEN_STRUCTURESMANAGERVIEW_HPP

// KF
#include <KNSCore/EntryInternal>
// Qt
#include <QWidget>

class StructuresSelector;
class QPushButton;

namespace KNSWidgets {
class Button;
}
namespace Kasten {
class StructuresManager;
class StructuresTool;
}

class StructuresManagerView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QStringList values READ values WRITE setValues NOTIFY changed USER true)

public:
    explicit StructuresManagerView(Kasten::StructuresTool* tool, QWidget* parent = nullptr);

    ~StructuresManagerView() override;

public:
    QStringList values() const;

public Q_SLOTS:
    void setValues(const QStringList& values);

Q_SIGNALS:
    void changed();

private Q_SLOTS:
    void onGetNewStructuresClicked(const QList<KNSCore::EntryInternal>& changedEntries);
    void advancedSelection();
    void setEnabledStructures(const QStringList& enabledStructures);

private:
    Kasten::StructuresTool* mTool;

    KNSWidgets::Button* mGetNewStructuresButton;
    QPushButton* mAdvancedSelectionButton;
    StructuresSelector* mStructuresSelector = nullptr;
};

#endif

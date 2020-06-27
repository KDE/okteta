/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREADDREMOVEWIDGET_HPP
#define KASTEN_STRUCTUREADDREMOVEWIDGET_HPP

#include <QWidget>

class QPushButton;
class QLabel;
class QTreeWidget;
class KListWidget;

namespace Kasten {
class StructuresTool;
}

class StructureAddRemoveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StructureAddRemoveWidget(const QStringList& selected, Kasten::StructuresTool* tool, QWidget* parent = nullptr);
    ~StructureAddRemoveWidget() override;

public:
    QStringList values() const;

private Q_SLOTS:
    void updateAvailable();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

private:
    void buildAvailableList();
    void syncData();

private:
    const Kasten::StructuresTool* mTool;

    QLabel* mTree1Label;
    QTreeWidget* mTreeAvailable;

    QLabel* mTree2Label;
    QTreeWidget* mTreeSelected;

    QPushButton* mRightButton;
    QPushButton* mLeftButton;

    QPushButton* mUpButton;
    QPushButton* mDownButton;

    QStringList mValues;
};

#endif // KASTEN_STRUCTUREADDREMOVEWIDGET_HPP

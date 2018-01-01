/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#ifndef STRUCTUREADDREMOVEWIDGET_H
#define STRUCTUREADDREMOVEWIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class QTreeWidget;
class KListWidget;

namespace Kasten
{
class StructuresTool;
}

class StructureAddRemoveWidget: public QWidget
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

#endif // STRUCTUREADDREMOVEWIDGET_H

/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
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

#include <QtGui/QWidget>

class KPushButton;
class QLabel;
class QTreeWidget;
class KListWidget;

namespace Kasten2
{
class StructTool;
}
class StructureAddRemoveWidget: public QWidget
{
Q_OBJECT

    Q_PROPERTY(QStringList values READ values USER true)
    QLabel* mTree1Label;
    QTreeWidget* mTreeAvailable;

    QLabel* mTree2Label;
    QTreeWidget* mTreeSelected;

    KPushButton* mRightButton;
    KPushButton* mLeftButton;

    KPushButton* mUpButton;
    KPushButton* mDownButton;

    QStringList mValues;
public:
    explicit StructureAddRemoveWidget(Kasten2::StructTool* tool, QWidget* parent = 0);
    ~StructureAddRemoveWidget();
    QStringList values() const
    {
        return mValues;
    }
Q_SIGNALS:
    void changed(const QStringList& newValues);
public Q_SLOTS:
    void updateAvailable();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
private:
    void buildAvailableList();
    void syncData();
    const Kasten2::StructTool* mTool;
};
#endif // STRUCTUREADDREMOVEWIDGET_H

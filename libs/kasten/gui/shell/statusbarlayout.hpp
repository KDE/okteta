/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_STATUSBARLAYOUT_HPP
#define KASTEN_STATUSBARLAYOUT_HPP

// Qt
#include <QLayout>
#include <QVector>

class QWidgetItem;

namespace Kasten {

class StatusBarLayout : public QLayout
{
    Q_OBJECT

public:
    explicit StatusBarLayout(QWidget* parent = nullptr);
    ~StatusBarLayout() override;

public:
    void addWidget(QWidget* widget);

public: // QLayout API
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    int indexOf(QWidget* widget) const override;

    void addItem(QLayoutItem* item) override;
    QLayoutItem* takeAt(int index) override;

public: // QLayoutItem API
    Qt::Orientations expandingDirections() const override;
    bool isEmpty() const override;
    QSize sizeHint() const override;
    QSize minimumSize () const override;

    void invalidate() override;
    void setGeometry(const QRect& rect) override;

private:
//     void updateMarginAndSpacing();
    void updateLayoutStructs() const;

private:
    QVector<QWidgetItem*> mWidgetList;

    bool mIsDirty : 1;
    bool mIsEmpty : 1;
    QSize mSizeHint;
};

}

#endif

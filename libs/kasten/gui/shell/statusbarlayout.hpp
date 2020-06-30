/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

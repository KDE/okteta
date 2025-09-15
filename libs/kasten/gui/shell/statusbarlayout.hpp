/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STATUSBARLAYOUT_HPP
#define KASTEN_STATUSBARLAYOUT_HPP

// Qt
#include <QLayout>
// Std
#include <vector>

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
    [[nodiscard]]
    int count() const override;
    [[nodiscard]]
    QLayoutItem* itemAt(int index) const override;
    [[nodiscard]]
    int indexOf(QWidget* widget) const override;

    void addItem(QLayoutItem* item) override;
    [[nodiscard]]
    QLayoutItem* takeAt(int index) override;

public: // QLayoutItem API
    [[nodiscard]]
    Qt::Orientations expandingDirections() const override;
    [[nodiscard]]
    bool isEmpty() const override;
    [[nodiscard]]
    QSize sizeHint() const override;
    [[nodiscard]]
    QSize minimumSize () const override;

    void invalidate() override;
    void setGeometry(const QRect& rect) override;

private:
//     void updateMarginAndSpacing();
    void updateLayoutStructs() const;

private:
    std::vector<QWidgetItem*> mWidgetList;

    bool mIsDirty : 1;
    bool mIsEmpty : 1;
    QSize mSizeHint;
};

}

#endif

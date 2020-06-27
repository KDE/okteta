/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2018 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TABBAR_HPP
#define KASTEN_TABBAR_HPP

// Qt
#include <QTabBar>

namespace Kasten {

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit TabBar(QWidget* parent = nullptr);
    ~TabBar() override;

Q_SIGNALS:
    void mouseMiddleClick();
    void testCanDecode(const QDragMoveEvent* event, bool& accept);
    void receivedDropEvent(QDropEvent* event);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};

}

#endif

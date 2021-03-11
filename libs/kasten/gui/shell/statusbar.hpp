/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STATUSBAR_HPP
#define KASTEN_STATUSBAR_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QStatusBar>

namespace Kasten {

class StatusBarLayout;

class KASTENGUI_EXPORT StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatusBar(QWidget* parent = nullptr);
    ~StatusBar() override;

public: // overwrite QStatusBar API
    void addWidget(QWidget* widget);

public:
    void updateLayout();

protected: // QWidget API
    void changeEvent(QEvent* changeEvent) override;

private:
    StatusBarLayout* mLayout;
};

}

#endif

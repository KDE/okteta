/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOGGLEBUTTON_HPP
#define KASTEN_TOGGLEBUTTON_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QToolButton>

namespace Kasten {

class KASTENGUI_EXPORT ToggleButton : public QToolButton
{
    Q_OBJECT

public:
    ToggleButton(const QString& text, const QString& toolTip, QWidget* parent = nullptr);
    ToggleButton(const QIcon& icon, const QString& text, const QString& toolTip, QWidget* parent = nullptr);
    ~ToggleButton() override;

public:
    void setCheckedState(const QIcon& icon, const QString& text, const QString& toolTip);
    void setCheckedState(const QString& text, const QString& toolTip);

private:
    const QScopedPointer<class ToggleButtonPrivate> d_ptr;
    Q_DECLARE_PRIVATE(ToggleButton)
};

}

#endif

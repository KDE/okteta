/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOGGLEBUTTON_P_HPP
#define KASTEN_TOGGLEBUTTON_P_HPP

// Qt
#include <QIcon>
#include <QString>

namespace Kasten {

class ToggleButton;

class ToggleButtonPrivate
{
public:
    explicit ToggleButtonPrivate(ToggleButton* parent);

public:
    void setOtherState(const QIcon& icon, const QString& text, const QString& toolTip);

public:
    void onToggled(bool);

private:
    QIcon mOtherIcon;
    QString mOtherText;
    QString mOtherToolTip;

    ToggleButton* const p;
};

}

#endif

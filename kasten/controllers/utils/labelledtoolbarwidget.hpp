/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef LABELLEDTOOLBARWIDGET_HPP
#define LABELLEDTOOLBARWIDGET_HPP

// Qt
#include <QWidget>

class QLabel;

namespace Kasten {

class LabelledToolBarWidget : public QWidget
{
    Q_OBJECT

public:
    LabelledToolBarWidget(QLabel* label,
                          QWidget* widget,
                          QWidget* parent);
};

}

#endif

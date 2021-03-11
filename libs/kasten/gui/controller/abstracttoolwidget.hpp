/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTTOOLWIDGET_HPP
#define KASTEN_ABSTRACTTOOLWIDGET_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QWidget>

class QPushButton;

namespace Kasten {

// TODO: this could also be implemented by some magic where the childWidgets are scanned for toolbuttons

class KASTENGUI_EXPORT AbstractToolWidget : public QWidget
{
    Q_OBJECT

public:
    enum DefaultType
    {
        AutoDefault,
        Default
    };

public:
    explicit AbstractToolWidget(QWidget* parent = nullptr);

    ~AbstractToolWidget() override;

public:
    // TODO: think about making this a factory template, similar to KActionCollection::addAction
    void addButton(QPushButton* button, DefaultType defaultType = AutoDefault);

private:
    const QScopedPointer<class AbstractToolWidgetPrivate> d_ptr;
    Q_DECLARE_PRIVATE(AbstractToolWidget)
};

}

#endif

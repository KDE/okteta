/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_WIDGETCOLUMNSTYLIST_P_HPP
#define OKTETA_WIDGETCOLUMNSTYLIST_P_HPP

// lib
#include "abstractcolumnstylist_p.hpp"

namespace Okteta {

class WidgetColumnStylistPrivate : public AbstractColumnStylistPrivate
{
public:
    explicit WidgetColumnStylistPrivate(QWidget* mWidget);
    ~WidgetColumnStylistPrivate();

public:
    QWidget* widget() const;

private:
    QWidget* const mWidget;
};

inline WidgetColumnStylistPrivate::WidgetColumnStylistPrivate(QWidget* widget) : mWidget(widget) {}
inline WidgetColumnStylistPrivate::~WidgetColumnStylistPrivate() = default;

inline QWidget* WidgetColumnStylistPrivate::widget() const { return mWidget; }

}

#endif

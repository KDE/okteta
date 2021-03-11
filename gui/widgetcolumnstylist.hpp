/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_WIDGETCOLUMNSTYLIST_HPP
#define OKTETA_WIDGETCOLUMNSTYLIST_HPP

// lib
#include "abstractcolumnstylist.hpp"

class QWidget;

namespace Okteta {

class WidgetColumnStylistPrivate;

class OKTETAGUI_EXPORT WidgetColumnStylist : public AbstractColumnStylist
{
public:
    explicit WidgetColumnStylist(QWidget* parent);
    ~WidgetColumnStylist() override;

public: // AbstractColumnStylist API
    const QPalette& palette() const override;

private:
    Q_DECLARE_PRIVATE(WidgetColumnStylist)
};

}

#endif

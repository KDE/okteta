/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWBOX_HPP
#define KASTEN_VIEWBOX_HPP

// Qt
#include <QWidget>

namespace Kasten {

class AbstractView;

class ViewBox : public QWidget
{
    Q_OBJECT

public:
    enum Area
    {
        TopArea,
        BottomArea
    };

public:
    explicit ViewBox(AbstractView* view, QWidget* parent = nullptr);
    ~ViewBox() override;

public:
    AbstractView* view() const;

public:
    void add(ViewBox::Area area);

private:
    AbstractView* mView;
};

}

#endif

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2021 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ICONLABEL_HPP
#define KASTEN_ICONLABEL_HPP

// Qt
#include <QLabel>
#include <QIcon>

namespace Kasten {

// TODO: fix QLabel to support icons, or rewrite to directly paint the icon ourselves
// still fails to update on icon theme change
class IconLabel : public QLabel
{
    Q_OBJECT

public:
    explicit IconLabel(QWidget* parent = nullptr);

    void setIcon(const QIcon& icon);

protected:
    void changeEvent(QEvent* event) override;

private:
    void updatePixmap();
    void updateSize();
    QSize iconSize() const;

private:
    QIcon mIcon;
};

}

#endif

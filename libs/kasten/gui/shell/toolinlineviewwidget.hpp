/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_TOOLINLINEVIEWWIDGET_HPP
#define KASTEN_TOOLINLINEVIEWWIDGET_HPP

// Qt
#include <QWidget>
#include <QPointer>

namespace Kasten {

class ToolInlineViewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolInlineViewWidget(QWidget* viewWidget, QWidget* parent = nullptr);
    ~ToolInlineViewWidget() override;

Q_SIGNALS:
    void done();

private:
    QPointer<QWidget> mViewWidget;
};

}

#endif

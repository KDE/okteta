/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VIEWAREABOX_HPP
#define KASTEN_VIEWAREABOX_HPP

// Qt
#include <QWidget>

class QShortcut;

namespace Kasten {

class ToolInlineViewWidget;

class ViewAreaBox : public QWidget
{
    Q_OBJECT

public:
    enum Area
    {
        TopArea,
        BottomArea
    };

public:
    /// does not take ownership of @p centralWidget, can be 0.
    explicit ViewAreaBox(QWidget* centralWidget, QWidget* parent = nullptr);
    ~ViewAreaBox() override;

public:
    QWidget* centralWidget() const;
    QWidget* bottomToolWidget() const;

public:
    /// does not take ownership of @p centralWidget, can be 0.
    void setCentralWidget(QWidget* centralWidget);
    /// does not take ownership of @p bottomWidget. Can be nullptr.
    void setBottomToolWidget(QWidget* bottomWidget);

    void showBottomToolWidget();

private Q_SLOTS:
    void onDone();

private:
    QWidget* mCentralWidget;

    QWidget* mBottomToolWidget = nullptr;
    ToolInlineViewWidget* mToolInlineViewWidget = nullptr;
    QShortcut* mEscapeShortcut;
};

}

#endif

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DUMMYVIEW_HPP
#define KASTEN_DUMMYVIEW_HPP

// lib
#include "abstractview.hpp"

class QLabel;

namespace Kasten {

class KASTENGUI_EXPORT DummyView : public AbstractView
{
    Q_OBJECT

public:
    explicit DummyView(AbstractDocument* document);

    ~DummyView() override;

public:
//     KCursorObject *cursor() const;

public: // AbstractModel API
    QString title() const override;

public: // AbstractView API
    void setFocus() override;
    QWidget* widget() const override;
    bool hasFocus() const override;

private Q_SLOTS:
    void onTitleChange(const QString& newTitle);

private:
    QLabel* mLabel;
    AbstractDocument* mDocument;
};

}

#endif

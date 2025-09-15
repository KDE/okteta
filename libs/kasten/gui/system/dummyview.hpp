/*
    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DUMMYVIEW_HPP
#define KASTEN_DUMMYVIEW_HPP

// lib
#include "abstractview.hpp"
// Std
#include <memory>

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
    [[nodiscard]]
    QString title() const override;

public: // AbstractView API
    void setFocus() override;
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    bool hasFocus() const override;

private Q_SLOTS:
    KASTENGUI_NO_EXPORT void onTitleChange(const QString& newTitle);

private:
    AbstractDocument* const mDocument;

    const std::unique_ptr<QLabel> mLabel;
};

}

#endif

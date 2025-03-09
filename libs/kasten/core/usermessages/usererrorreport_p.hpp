/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERERRORREPORT_P_HPP
#define KASTEN_USERERRORREPORT_P_HPP

// lib
#include "usererrorreport.hpp"

namespace Kasten {

class UserErrorReportPrivate
{
public:
    explicit UserErrorReportPrivate(AbstractModel* targetModel, const QString& text, const QString& title);
    UserErrorReportPrivate(const UserErrorReportPrivate&) = delete;
    UserErrorReportPrivate(UserErrorReportPrivate&&) = delete;

    ~UserErrorReportPrivate();

    UserErrorReportPrivate& operator=(const UserErrorReportPrivate&) = delete;
    UserErrorReportPrivate& operator=(UserErrorReportPrivate&&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    const QString& text() const;
    [[nodiscard]]
    const QString& title() const;

private:
    AbstractModel* const m_targetModel;
    const QString m_text;
    const QString m_title;
};

inline UserErrorReportPrivate::UserErrorReportPrivate(AbstractModel* targetModel, const QString& text, const QString& title)
    : m_targetModel(targetModel)
    , m_text(text)
    , m_title(title)
{
}

inline UserErrorReportPrivate::~UserErrorReportPrivate() = default;

inline AbstractModel* UserErrorReportPrivate::targetModel() const { return m_targetModel; }
inline const QString& UserErrorReportPrivate::text() const { return m_text; }
inline const QString& UserErrorReportPrivate::title() const { return m_title; }

}

#endif

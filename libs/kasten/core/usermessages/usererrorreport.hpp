/*
    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_USERERRORREPORT_HPP
#define KASTEN_USERERRORREPORT_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QtClassHelperMacros> // Q_DECLARE_PRIVATE
// Q_CAST_IGNORE_ALIGN, used by Q_DECLARE_PRIVATE but without needed include
#include <QtCompilerDetection> // IWYU pragma: keep
#include <QString>
// Std
#include <memory>

namespace Kasten {

class AbstractModel;

class KASTENCORE_EXPORT UserErrorReport
{
public:
    explicit UserErrorReport(AbstractModel* targetModel, const QString& text, const QString& title = QString());
    UserErrorReport(const UserErrorReport&) = delete;
    UserErrorReport(UserErrorReport&&) = delete;

    ~UserErrorReport();

public:
    UserErrorReport& operator=(const UserErrorReport&) = delete;
    UserErrorReport& operator=(UserErrorReport&&) = delete;

public:
    [[nodiscard]]
    AbstractModel* targetModel() const;
    [[nodiscard]]
    QString text() const;
    [[nodiscard]]
    QString title() const;

private:
    const std::unique_ptr<class UserErrorReportPrivate> d_ptr;
    Q_DECLARE_PRIVATE(UserErrorReport)
};

}

#endif

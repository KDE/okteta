/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2024 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_USERERRORREPORTSINLINEABLE_HPP
#define KASTEN_IF_USERERRORREPORTSINLINEABLE_HPP

// Qt
#include <QtPlugin>
// Std
#include <memory>

namespace Kasten {

class UserErrorReport;

namespace If {

class UserErrorReportsInlineable
{
public:
    virtual ~UserErrorReportsInlineable();

public: // actions
    virtual void showErrorReport(UserErrorReport* errorReport) = 0;

public: // get
    [[nodiscard]]
    virtual bool isErrorReportShown() const = 0;

public: // signal
    virtual void errorReportHidden() = 0;
};

inline UserErrorReportsInlineable::~UserErrorReportsInlineable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::UserErrorReportsInlineable, "org.kde.kasten.if.usererrorreportsinlineable/1.0")

#endif

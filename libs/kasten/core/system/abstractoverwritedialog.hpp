/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTOVERWRITEDIALOG_HPP
#define KASTEN_ABSTRACTOVERWRITEDIALOG_HPP

// lib
#include "kastencore_export.hpp"
#include "kastencore.hpp"

class QUrl;
class QString;

namespace Kasten {

class KASTENCORE_EXPORT AbstractOverwriteDialog
{
public:
    virtual ~AbstractOverwriteDialog();

public: // API to be implemented
    virtual Answer queryOverwrite(const QUrl& url, const QString& title) const = 0;
};

inline AbstractOverwriteDialog::~AbstractOverwriteDialog() = default;

}

#endif

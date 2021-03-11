/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTSAVEDISCARDDIALOG_HPP
#define KASTEN_ABSTRACTSAVEDISCARDDIALOG_HPP

// lib
#include "kastencore_export.hpp"
#include "kastencore.hpp"

class QString;

namespace Kasten {
class AbstractDocument;

class KASTENCORE_EXPORT AbstractSaveDiscardDialog
{
public:
    virtual ~AbstractSaveDiscardDialog();

public: // API to be implemented
    virtual Answer queryDiscardOnReload(const AbstractDocument* document, const QString& title) const = 0;
    virtual Answer querySaveDiscard(const AbstractDocument* document, const QString& title) const = 0;
    virtual Answer queryDiscard(const AbstractDocument* document, const QString& title) const = 0;
};

inline AbstractSaveDiscardDialog::~AbstractSaveDiscardDialog() = default;

}

#endif

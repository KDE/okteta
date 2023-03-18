/*
    This file is part of the Okteta Gui library, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTCOLUMNSTYLIST_HPP
#define OKTETA_ABSTRACTCOLUMNSTYLIST_HPP

// lib
#include "oktetagui_export.hpp"
// Qt
#include <QtGlobal>
// Std
#include <memory>

class QPalette;

namespace Okteta {

class AbstractColumnStylistPrivate;

class OKTETAGUI_EXPORT AbstractColumnStylist
{
protected:
    OKTETAGUI_NO_EXPORT explicit AbstractColumnStylist(AbstractColumnStylistPrivate* d);

public:
    AbstractColumnStylist();
    virtual ~AbstractColumnStylist();

public: // API to be reimplemented in the subclasses
    virtual const QPalette& palette() const = 0;

protected:
    const std::unique_ptr<AbstractColumnStylistPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractColumnStylist)
};

}

#endif

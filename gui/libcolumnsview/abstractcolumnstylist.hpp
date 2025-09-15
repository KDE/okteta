/*
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
    OKTETAGUI_NO_EXPORT explicit AbstractColumnStylist(std::unique_ptr<AbstractColumnStylistPrivate>&& dd);

public:
    AbstractColumnStylist();
    AbstractColumnStylist(const AbstractColumnStylist&) = delete;
    AbstractColumnStylist(AbstractColumnStylist&&) = delete;

    virtual ~AbstractColumnStylist();

    AbstractColumnStylist& operator=(const AbstractColumnStylist&) = delete;
    AbstractColumnStylist& operator=(AbstractColumnStylist&&) = delete;

public: // API to be reimplemented in the subclasses
    [[nodiscard]]
    virtual const QPalette& palette() const = 0;

protected:
    const std::unique_ptr<AbstractColumnStylistPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractColumnStylist)
};

}

#endif

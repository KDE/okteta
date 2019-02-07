/*
    This file is part of the Okteta Gui library, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OKTETA_ABSTRACTCOLUMNSTYLIST_HPP
#define OKTETA_ABSTRACTCOLUMNSTYLIST_HPP

// lib
#include <okteta/oktetagui_export.hpp>
// Qt
#include <QtGlobal>
#include <QScopedPointer>

class QPalette;

namespace Okteta {

class AbstractColumnStylistPrivate;

class OKTETAGUI_EXPORT AbstractColumnStylist
{
public:
    AbstractColumnStylist();
    virtual ~AbstractColumnStylist();

public: // API to be reimplemented in the subclasses
    virtual const QPalette& palette() const = 0;

protected:
    explicit AbstractColumnStylist(AbstractColumnStylistPrivate* d);

protected:
    const QScopedPointer<AbstractColumnStylistPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractColumnStylist)
};

}

#endif

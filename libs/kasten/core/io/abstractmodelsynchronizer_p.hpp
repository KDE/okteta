/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_ABSTRACTMODELSYNCHRONIZER_P_HPP
#define KASTEN_ABSTRACTMODELSYNCHRONIZER_P_HPP

#include "abstractmodelsynchronizer.hpp"

// Qt
#include <QUrl>

namespace Kasten {

class AbstractModelSynchronizerPrivate
{
public:
    explicit AbstractModelSynchronizerPrivate(AbstractModelSynchronizer* parent);

    virtual ~AbstractModelSynchronizerPrivate();

public:
    const QUrl& url() const;

public:
    void setUrl(const QUrl& url);

protected:
    AbstractModelSynchronizer* const q_ptr;

protected:
    QUrl mUrl;

private:
    Q_DECLARE_PUBLIC(AbstractModelSynchronizer)
};

inline AbstractModelSynchronizerPrivate::AbstractModelSynchronizerPrivate(AbstractModelSynchronizer* parent)
    : q_ptr(parent)
{
}

inline AbstractModelSynchronizerPrivate::~AbstractModelSynchronizerPrivate() = default;

inline const QUrl& AbstractModelSynchronizerPrivate::url() const { return mUrl; }

inline void AbstractModelSynchronizerPrivate::setUrl(const QUrl& url)
{
    Q_Q(AbstractModelSynchronizer);

    mUrl = url;
    Q_EMIT q->urlChanged(url);
}

}

#endif

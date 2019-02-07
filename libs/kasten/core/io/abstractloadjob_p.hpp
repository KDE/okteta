/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_ABSTRACTLOADJOB_P_HPP
#define KASTEN_ABSTRACTLOADJOB_P_HPP

// lib
#include "abstractloadjob.hpp"

namespace Kasten {

class AbstractLoadJobPrivate
{
public:
    explicit AbstractLoadJobPrivate(AbstractLoadJob* parent);
    AbstractLoadJobPrivate() = delete;

    virtual ~AbstractLoadJobPrivate();

public:
    AbstractDocument* document() const;

public:
    void setDocument(AbstractDocument* document);

protected:
    Q_DECLARE_PUBLIC(AbstractLoadJob)

protected:
    AbstractLoadJob* const q_ptr;

    AbstractDocument* mDocument = nullptr;
};

inline AbstractLoadJobPrivate::AbstractLoadJobPrivate(AbstractLoadJob* parent)
    : q_ptr(parent)
{}

inline AbstractLoadJobPrivate::~AbstractLoadJobPrivate() = default;

inline AbstractDocument* AbstractLoadJobPrivate::document() const { return mDocument; }
inline void AbstractLoadJobPrivate::setDocument(AbstractDocument* document)
{
    Q_Q(AbstractLoadJob);

    if (document) {
        mDocument = document;
        emit q->documentLoaded(document);
    }

    q->emitResult();
}

}

#endif

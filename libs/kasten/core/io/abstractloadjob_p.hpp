/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    AbstractLoadJob* const q_ptr;

private:
    Q_DECLARE_PUBLIC(AbstractLoadJob)

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

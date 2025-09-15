/*
    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTLOADJOB_P_HPP
#define KASTEN_ABSTRACTLOADJOB_P_HPP

// lib
#include "abstractloadjob.hpp"
#include "abstractdocument.hpp"

namespace Kasten {

class AbstractLoadJobPrivate
{
public:
    explicit AbstractLoadJobPrivate(AbstractLoadJob* parent);
    AbstractLoadJobPrivate(const AbstractLoadJobPrivate&) = delete;
    AbstractLoadJobPrivate(AbstractLoadJobPrivate&&) = delete;

    virtual ~AbstractLoadJobPrivate();

    AbstractLoadJobPrivate& operator=(const AbstractLoadJobPrivate&) = delete;
    AbstractLoadJobPrivate& operator=(AbstractLoadJobPrivate&&) = delete;

public:
    [[nodiscard]]
    std::unique_ptr<AbstractDocument> releaseDocument();

public:
    void setDocument(std::unique_ptr<AbstractDocument>&& document);

protected:
    AbstractLoadJob* const q_ptr;

private:
    Q_DECLARE_PUBLIC(AbstractLoadJob)

    std::unique_ptr<AbstractDocument> mDocument;
};

inline AbstractLoadJobPrivate::AbstractLoadJobPrivate(AbstractLoadJob* parent)
    : q_ptr(parent)
{}

inline AbstractLoadJobPrivate::~AbstractLoadJobPrivate() = default;

inline std::unique_ptr<AbstractDocument> AbstractLoadJobPrivate::releaseDocument() { return std::move(mDocument); }
inline void AbstractLoadJobPrivate::setDocument(std::unique_ptr<AbstractDocument>&& document)
{
    Q_Q(AbstractLoadJob);

    mDocument = std::move(document);

    q->emitResult();
}

}

#endif

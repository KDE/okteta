/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTEXPORTJOB_P_HPP
#define KASTEN_ABSTRACTEXPORTJOB_P_HPP

#include "abstractexportjob.hpp"

namespace Kasten {

class AbstractExportJobPrivate
{
public:
    explicit AbstractExportJobPrivate(AbstractExportJob* parent);
    AbstractExportJobPrivate() = delete;

    virtual ~AbstractExportJobPrivate();

public:
    AbstractDocument* document() const;
    void setDocument(AbstractDocument* document);

protected:
    AbstractExportJob* const q_ptr;

    AbstractDocument* mDocument = nullptr;
};

inline AbstractExportJobPrivate::AbstractExportJobPrivate(AbstractExportJob* parent)
    : q_ptr(parent)
{}

inline AbstractExportJobPrivate::~AbstractExportJobPrivate() = default;

inline AbstractDocument* AbstractExportJobPrivate::document() const { return mDocument; }
inline void AbstractExportJobPrivate::setDocument(AbstractDocument* document)
{
    mDocument = document;

    emit q_ptr->documentLoaded(document);
    q_ptr->emitResult();
}

}

#endif

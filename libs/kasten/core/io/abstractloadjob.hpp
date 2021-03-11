/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTLOADJOB_HPP
#define KASTEN_ABSTRACTLOADJOB_HPP

// lib
#include "kastencore_export.hpp"
// KF
#include <KJob>

namespace Kasten {

class AbstractDocument;

class AbstractLoadJobPrivate;

class KASTENCORE_EXPORT AbstractLoadJob : public KJob
{
    Q_OBJECT

protected:
    explicit AbstractLoadJob(AbstractLoadJobPrivate* d);

public:
    AbstractLoadJob();

    ~AbstractLoadJob() override;

public:
    AbstractDocument* document() const;

Q_SIGNALS:
    void documentLoaded(Kasten::AbstractDocument* document);

protected:
    // emits documentLoaded()
    // TODO: or better name property LoadedDocument?
    virtual void setDocument(AbstractDocument* document);

protected:
    const QScopedPointer<AbstractLoadJobPrivate> d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractLoadJob)
};

}

#endif

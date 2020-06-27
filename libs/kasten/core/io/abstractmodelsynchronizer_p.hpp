/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    emit q->urlChanged(url);
}

}

#endif

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayfilter.hpp"

// Qt
#include <QString>

class AbstractByteArrayFilterPrivate
{
public:
    explicit AbstractByteArrayFilterPrivate(const QString& name);

public:
    QString name() const;

private:
    const QString mName;
};

inline AbstractByteArrayFilterPrivate::AbstractByteArrayFilterPrivate(const QString& name)
    : mName(name)
{}

inline QString AbstractByteArrayFilterPrivate::name() const { return mName; }

AbstractByteArrayFilter::AbstractByteArrayFilter(const QString& name)
    : d(new AbstractByteArrayFilterPrivate(name))
{}

AbstractByteArrayFilter::~AbstractByteArrayFilter() = default;

QString AbstractByteArrayFilter::name() const { return d->name(); }

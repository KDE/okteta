/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearrayfilter.hpp"

// Qt
#include <QString>

class AbstractByteArrayFilterPrivate
{
public:
    explicit AbstractByteArrayFilterPrivate(const QString& name, const QString& id);

public:
    const QString& name() const;
    const QString& id() const;

private:
    const QString mName;
    const QString mId;
};

inline AbstractByteArrayFilterPrivate::AbstractByteArrayFilterPrivate(const QString& name, const QString& id)
    : mName(name)
    , mId(id)
{}

inline const QString& AbstractByteArrayFilterPrivate::name() const { return mName; }
inline const QString& AbstractByteArrayFilterPrivate::id() const { return mId; }

AbstractByteArrayFilter::AbstractByteArrayFilter(const QString& name, const QString& id)
    : d(new AbstractByteArrayFilterPrivate(name, id))
{}

AbstractByteArrayFilter::~AbstractByteArrayFilter() = default;

void AbstractByteArrayFilter::loadConfig(const KConfigGroup& configGroup)
{
    Q_UNUSED(configGroup);
}

void AbstractByteArrayFilter::saveConfig(KConfigGroup& configGroup) const
{
    Q_UNUSED(configGroup);
}

QString AbstractByteArrayFilter::name() const { return d->name(); }
QString AbstractByteArrayFilter::id() const { return d->id(); }

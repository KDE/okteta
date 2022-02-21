/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraychecksumalgorithm.hpp"

// Qt
#include <QString>

class AbstractByteArrayChecksumAlgorithmPrivate
{
public:
    explicit AbstractByteArrayChecksumAlgorithmPrivate(const QString& name, const QString& id);

public:
    const QString& name() const;
    const QString& id() const;

private:
    const QString mName;
    const QString mId;
};

AbstractByteArrayChecksumAlgorithmPrivate::AbstractByteArrayChecksumAlgorithmPrivate(const QString& name, const QString& id)
    : mName(name)
    , mId(id)
{}

inline const QString& AbstractByteArrayChecksumAlgorithmPrivate::name() const { return mName; }
inline const QString& AbstractByteArrayChecksumAlgorithmPrivate::id() const { return mId; }

AbstractByteArrayChecksumAlgorithm::AbstractByteArrayChecksumAlgorithm(const QString& name, const QString& id)
    : d(new AbstractByteArrayChecksumAlgorithmPrivate(name, id))
{}

AbstractByteArrayChecksumAlgorithm::~AbstractByteArrayChecksumAlgorithm() = default;

void AbstractByteArrayChecksumAlgorithm::loadConfig(const KConfigGroup& configGroup)
{
    Q_UNUSED(configGroup);
}

void AbstractByteArrayChecksumAlgorithm::saveConfig(KConfigGroup& configGroup) const
{
    Q_UNUSED(configGroup);
}

QString AbstractByteArrayChecksumAlgorithm::name() const { return d->name(); }
QString AbstractByteArrayChecksumAlgorithm::id() const { return d->id(); }

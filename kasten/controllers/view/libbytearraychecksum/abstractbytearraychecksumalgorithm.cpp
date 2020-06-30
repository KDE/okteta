/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "abstractbytearraychecksumalgorithm.hpp"

// Qt
#include <QString>

class AbstractByteArrayChecksumAlgorithmPrivate
{
public:
    explicit AbstractByteArrayChecksumAlgorithmPrivate(const QString& name);

public:
    QString name() const;

private:
    const QString mName;
};

AbstractByteArrayChecksumAlgorithmPrivate::AbstractByteArrayChecksumAlgorithmPrivate(const QString& name)
    : mName(name)
{}

inline QString AbstractByteArrayChecksumAlgorithmPrivate::name() const { return mName; }

AbstractByteArrayChecksumAlgorithm::AbstractByteArrayChecksumAlgorithm(const QString& name)
    : d(new AbstractByteArrayChecksumAlgorithmPrivate(name))
{}

AbstractByteArrayChecksumAlgorithm::~AbstractByteArrayChecksumAlgorithm() = default;

QString AbstractByteArrayChecksumAlgorithm::name() const { return d->name(); }

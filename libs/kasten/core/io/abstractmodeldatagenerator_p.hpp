/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODELDATAGENERATOR_P_HPP
#define KASTEN_ABSTRACTMODELDATAGENERATOR_P_HPP

#include "abstractmodeldatagenerator.hpp"

namespace Kasten {

class AbstractModelDataGeneratorPrivate
{
public:
    explicit AbstractModelDataGeneratorPrivate(AbstractModelDataGenerator* parent,
                                               const QString& typeName, const QString& mimeType,
                                               AbstractModelDataGenerator::Flags flags);

    virtual ~AbstractModelDataGeneratorPrivate();

public:
    const QString& typeName() const;
    const QString& mimeType() const;
    AbstractModelDataGenerator::Flags flags() const;

protected:
    AbstractModelDataGenerator* const q_ptr;

    const QString mTypeName;
    const QString mMimeType;
    AbstractModelDataGenerator::Flags mFlags;
};

inline AbstractModelDataGeneratorPrivate::AbstractModelDataGeneratorPrivate(AbstractModelDataGenerator* parent,
                                                                            const QString& typeName, const QString& mimeType, AbstractModelDataGenerator::Flags flags)
    : q_ptr(parent)
    , mTypeName(typeName)
    , mMimeType(mimeType)
    , mFlags(flags)
{}

inline AbstractModelDataGeneratorPrivate::~AbstractModelDataGeneratorPrivate() = default;

inline const QString& AbstractModelDataGeneratorPrivate::typeName() const { return mTypeName; }
inline const QString& AbstractModelDataGeneratorPrivate::mimeType() const { return mMimeType; }
inline AbstractModelDataGenerator::Flags AbstractModelDataGeneratorPrivate::flags() const { return mFlags; }

}

#endif

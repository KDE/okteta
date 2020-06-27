/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_ABSTRACTBYTEARRAYMODEL_P_HPP
#define OKTETA_ABSTRACTBYTEARRAYMODEL_P_HPP

// lib
#include "abstractbytearraymodel.hpp"

namespace Okteta {

class AbstractByteArrayModelPrivate
{
public:
    explicit AbstractByteArrayModelPrivate(AbstractByteArrayModel* parent);
    AbstractByteArrayModelPrivate() = delete;
    virtual ~AbstractByteArrayModelPrivate();

protected:
    AbstractByteArrayModel* const q_ptr;

private:
    Q_DECLARE_PUBLIC(AbstractByteArrayModel)
};


inline AbstractByteArrayModelPrivate::AbstractByteArrayModelPrivate(AbstractByteArrayModel* parent)
    : q_ptr(parent)
{
}

inline AbstractByteArrayModelPrivate::~AbstractByteArrayModelPrivate() = default;

}

#endif

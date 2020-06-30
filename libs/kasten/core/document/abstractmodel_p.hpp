/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTMODEL_P_HPP
#define KASTEN_ABSTRACTMODEL_P_HPP

#include "abstractmodel.hpp"

namespace Kasten {

class AbstractModelPrivate
{
public:
    explicit AbstractModelPrivate(AbstractModel* parent, AbstractModel* baseModel = nullptr);

    virtual ~AbstractModelPrivate();

public:
    AbstractModel* baseModel() const;

public:
    void setBaseModel(AbstractModel* baseModel);

protected:
    AbstractModel* const q_ptr;

private:
    Q_DECLARE_PUBLIC(AbstractModel)

    AbstractModel* mBaseModel;
};

inline AbstractModelPrivate::AbstractModelPrivate(AbstractModel* parent, AbstractModel* baseModel)
    : q_ptr(parent)
    , mBaseModel(baseModel)
{}

inline AbstractModelPrivate::~AbstractModelPrivate() = default;

inline AbstractModel* AbstractModelPrivate::baseModel() const { return mBaseModel; }
inline void AbstractModelPrivate::setBaseModel(AbstractModel* baseModel) { mBaseModel = baseModel; }

}

#endif

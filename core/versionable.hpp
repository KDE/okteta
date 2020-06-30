/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_VERSIONABLE_HPP
#define OKTETA_VERSIONABLE_HPP

// Qt
#include <QtPlugin>

class QString;

namespace Okteta {

class Versionable
{
public:
    virtual ~Versionable();

public: // get
    virtual int versionIndex() const = 0;
    virtual int versionCount() const = 0;
    virtual QString versionDescription(int versionIndex) const = 0;

public: // set/action
    virtual void revertToVersionByIndex(int versionIndex) = 0;

public: // signal
    virtual void revertedToVersionIndex(int versionIndex) = 0;
    virtual void headVersionDescriptionChanged(const QString& versionDescription) = 0;
    virtual void headVersionChanged(int newHeadVersionIndex) = 0;
};

inline Versionable::~Versionable() = default;

}

Q_DECLARE_INTERFACE(Okteta::Versionable, "org.kde.okteta.versionable/1.0")

#endif

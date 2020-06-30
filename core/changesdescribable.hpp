/*
    This file is part of the Okteta Core library, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef OKTETA_CHANGESDESCRIBABLE_HPP
#define OKTETA_CHANGESDESCRIBABLE_HPP

// Qt
#include <QtPlugin>
#include <QString>

namespace Okteta {

class ChangesDescribable
{
public:
    virtual ~ChangesDescribable();

public: // set/action
    virtual void openGroupedChange(const QString& description = QString()) = 0;
    virtual void cancelGroupedChange() = 0;
    virtual void closeGroupedChange(const QString& description = QString()) = 0;

public: // get

public: // signal
};

inline ChangesDescribable::~ChangesDescribable() = default;

}

Q_DECLARE_INTERFACE(Okteta::ChangesDescribable, "org.kde.okteta.changesdescribable/1.0")

#endif

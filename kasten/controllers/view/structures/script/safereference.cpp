/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2013 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "safereference.hpp"
#include "../structlogging.hpp"
// #include "../datatypes/datainformation.hpp"
// Std
#include <cstdio>

SafeReferenceHolder SafeReferenceHolder::instance;

SafeReferenceHolder::SafeReferenceHolder() = default;

SafeReferenceHolder::~SafeReferenceHolder()
{
    if (!mRefs.isEmpty()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << mRefs.size() << "safe references were not removed";
    }
#ifndef NDEBUG
   printf("register count: %d, destroy count: %d\n", safeRefRegisterCnt, safeRefDestroyCnt);
#endif
}

void SafeReferenceHolder::invalidateAll(DataInformation* data)
{
    // this is called from DataInformation destructor, don't do anything with data!
    Container::iterator i = mRefs.find(data);
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "invalidating all references to" << data->name();
    while (i != mRefs.end() && i.key() == data) {
        // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "invalidating" << i.value();
        i.value()->invalidate();
        ++i;
    }
    // remove all this items from the list
    int removed = mRefs.remove(data);
    safeRefDestroyCnt += removed;
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "removed" << removed << "items";
}

/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2013 Alex Richardson <alex.richardson@gmx.de>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
#include "safereference.h"
//#include "../datatypes/datainformation.h"

#include <KDebug>
#include <cstdio>

SafeReferenceHolder SafeReferenceHolder::instance = SafeReferenceHolder();

void SafeReferenceHolder::invalidateAll(DataInformation* data)
{
    //this is called from DataInformation destructor, don't do anything with data!
    Container::iterator i = mRefs.find(data);
    //qDebug() << "invalidating all references to" << data->name();
    while (i != mRefs.end() && i.key() == data) {
        //qDebug() << "invalidating" << i.value();
        i.value()->invalidate();
        ++i;
    }
    //remove all this items from the list
    int removed = mRefs.remove(data);
    safeRefDestroyCnt += removed;
    //qDebug() << "removed" << removed << "items";
}

SafeReferenceHolder::SafeReferenceHolder()
    : safeRefDestroyCnt(0), safeRefRegisterCnt(0)
{
}

SafeReferenceHolder::~SafeReferenceHolder()
{
    if (mRefs.size() > 0)
        kWarning() << mRefs.size() << "safe references were not removed";
    printf("register count: %d, destroy count %d: ", safeRefRegisterCnt, safeRefDestroyCnt);
}




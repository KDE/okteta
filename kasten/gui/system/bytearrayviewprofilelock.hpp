/*
    SPDX-FileCopyrightText: 2011-2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTEARRAYVIEWPROFILELOCK_HPP
#define KASTEN_BYTEARRAYVIEWPROFILELOCK_HPP

// lib
#include "bytearrayviewprofile.hpp"
// Std
#include <memory>

class QString;

namespace Kasten {
class ByteArrayViewProfileLockPrivate;

class OKTETAKASTENGUI_EXPORT ByteArrayViewProfileLock
{
    friend class ByteArrayViewProfileManager;

protected:
    ByteArrayViewProfileLock(const QString& fileName,
                             const ByteArrayViewProfile::Id& viewProfileId);

public:
    ByteArrayViewProfileLock(const ByteArrayViewProfileLock& other);
    ByteArrayViewProfileLock(ByteArrayViewProfileLock&& other);

    ~ByteArrayViewProfileLock();

public:
    ByteArrayViewProfileLock& operator=(const ByteArrayViewProfileLock& other);
    ByteArrayViewProfileLock& operator=(ByteArrayViewProfileLock&& other);

public:
    // TODO: check if thread-safeness is needed
    void unlock();
    [[nodiscard]]
    bool isLocked() const;
    [[nodiscard]]
    ByteArrayViewProfile::Id viewProfileId() const;

private:
    std::shared_ptr<ByteArrayViewProfileLockPrivate> d;
};

}

#endif

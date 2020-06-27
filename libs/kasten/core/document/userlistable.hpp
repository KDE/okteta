/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_IF_USERLISTABLE_HPP
#define KASTEN_IF_USERLISTABLE_HPP

// Qt
#include <QtPlugin>

template <class T> class QVector;

namespace Kasten {

class Person;

namespace If {

// TODO: can owner change? Later perhaps, if storage will be movable
class UserListable
{
public:
    virtual ~UserListable();

public: // get
    virtual Person owner() const = 0;
    virtual QVector<Person> userList() const = 0;

public: // set/action
//     virtual void setVersion( DocumentVersionId id ) = 0;
//     virtual void revertToVersionByIndex( int versionIndex ) = 0;

public: // signal
//     virtual void ownerChange( const Person& newOwner ) = 0;
    virtual void usersAdded(const QVector<Person>& newUserList) = 0;
    virtual void usersRemoved(const QVector<Person>& newUserList) = 0;
};

inline UserListable::~UserListable() = default;

}
}

Q_DECLARE_INTERFACE(Kasten::If::UserListable, "org.kde.kasten.if.userlistable/1.0")

#endif

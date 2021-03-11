/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PERSON_HPP
#define KASTEN_PERSON_HPP

// lib
#include "kastencore_export.hpp"
// Qt
#include <QExplicitlySharedDataPointer>

class QIcon;
class QString;

namespace Kasten {

class KASTENCORE_EXPORT Person
{
public: // TODO: remove me again, hack!
    static Person createEgo();
    static void setEgoId(int egoId);

public:
    Person(const QString& name, const QIcon& faceIcon);
    Person();
    Person(const Person& other);
    ~Person();

public:
    Person& operator=(const Person& other);

public:
    bool operator==(const Person& other) const;

public:
    QString name() const;
    QIcon faceIcon() const;

private:
    QExplicitlySharedDataPointer<class PersonPrivate> d;
};

}

Q_DECLARE_TYPEINFO(Kasten::Person, Q_MOVABLE_TYPE);

#endif

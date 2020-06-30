/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "person.hpp"
#include "person_p.hpp"

namespace Kasten {

static constexpr struct EgoDataStruct
{
    const char* name;
    const char* faceIconName;
}
EgoData[] =
{
    {"Ego", "face-smile"},
    {"Joe Developer", "face-surprise"},
    {"Konqui", "face-laugh"},
    {"Hans Entwickler", "user-identity"}
};
static constexpr int lastEgoDataIndex = sizeof(EgoData) / sizeof(EgoDataStruct) - 1;
static int currentEgoDataIndex = 0;

Person Person::createEgo()
{
    const EgoDataStruct* currentEgoData = &EgoData[currentEgoDataIndex];
    const Person result(QLatin1String(currentEgoData->name),
                        QIcon::fromTheme(QLatin1String(currentEgoData->faceIconName)));
//     if( currentEgoDataIndex < lastEgoDataIndex )
//         ++currentEgoDataIndex;
    return result;
}

void Person::setEgoId(int egoId)
{
    if (lastEgoDataIndex < egoId) {
        egoId = lastEgoDataIndex;
    }
    currentEgoDataIndex = egoId;
}

Person::Person(const QString& name, const QIcon& faceIcon)
    : d(new PersonPrivate(name, faceIcon))
{
}
Person::Person()
    : d(new PersonPrivate(QString(), QIcon()))
{
}
Person::Person(const Person& other) = default;

Person::~Person() = default;

Person& Person::operator=(const Person& other) = default;

bool Person::operator==(const Person& other) const
{
    return (name() == other.name()) && !name().isEmpty();
}

QString Person::name()   const { return d->name(); }
QIcon Person::faceIcon() const { return d->faceIcon(); }

}

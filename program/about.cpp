/*
    This file is part of the Okteta program, made within the KDE community.

    Copyright 2006-2014 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License or (at your option) version 3 or any later version
    accepted by the membership of KDE e.V. (or its successor approved
    by the membership of KDE e.V.), which shall act as a proxy
    defined in Section 14 of version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "about.h"

// KF5
#include <KLocalizedString>


OktetaAboutData::OktetaAboutData()
: KAboutData( QStringLiteral("okteta"),
              i18n("Okteta"), // name
              QStringLiteral("0.16.0"),
              i18n("Hex editor"), // description
              KAboutLicense::GPL_V2,
              i18n("Copyright 2006-2015 Friedrich W. H. Kossebau"), //copyright
              i18n("Edit the raw data of files"), // comment
              QStringLiteral("http://userbase.kde.org/Okteta") )
{
    addLicense(KAboutLicense::GPL_V3);
    setOrganizationDomain( "kde.org" );
    addAuthor( i18n("Friedrich W. H. Kossebau"), // name
               i18n("Author"), // task
               QStringLiteral("kossebau@kde.org") );
    addAuthor( i18nc("Author","Alexander Richardson"),// name
               i18nc("Task description","Structures tool"), // task
               QStringLiteral("alex.richardson@gmx.de") );
}

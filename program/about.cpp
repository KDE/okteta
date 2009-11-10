/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// KDE
#include <KLocale>

#include <kdeversion.h>


// Program
static const char ProgramId[] =          "okteta";
static const char ProgramVersion[] =     "0.3.4";
static const char ProgramHomepage[] =    "http://utils.kde.org/projects/okteta";
// Author
static const char FWHKEmailAddress[] =   "kossebau@kde.org";


OktetaAboutData::OktetaAboutData()
: KAboutData( ProgramId, 0,
              ki18n("Okteta"), ProgramVersion, // name
              ki18n("KDE byte editor"), // description
              KAboutData::License_GPL_V2,
              ki18n("Copyright 2006-2009 Friedrich W. H. Kossebau"), //copyright
              ki18n("Edit the raw data of files"), // comment
              ProgramHomepage )
{
    addLicense(KAboutData::License_GPL_V3);
    setOrganizationDomain( "kde.org" );
    addAuthor( ki18n("Friedrich W. H. Kossebau"), // name
               ki18n("Author"), // task
               FWHKEmailAddress );
}

/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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
static const char ProgramVersion[] =     "0.1.1";
static const char ProgramHomepage[] =    "http://utils.kde.org/projects/okteta";
// Author
static const char FWHKEmailAddress[] =   "kossebau@kde.org";


OktetaAboutData::OktetaAboutData()
: KAboutData( ProgramId, 0,
              ki18n("Okteta"), ProgramVersion, // name
              ki18n("KDE bytes editor"), // description
              KAboutData::License_GPL_V2,
              ki18n("Copyright 2006-2008 Friedrich W. H. Kossebau"), //copyright
              ki18n("Edit files as an array of bytes"), // comment
              ProgramHomepage )
{
#if KDE_IS_VERSION(4,0,70)
    addLicense(KAboutData::License_GPL_V3);
#endif
    setOrganizationDomain( "kde.org" );
    addAuthor( ki18n("Friedrich W. H. Kossebau"), // name
               ki18n("Author"), // task
               FWHKEmailAddress );
}

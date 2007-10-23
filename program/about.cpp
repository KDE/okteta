/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2 as
    published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/


#include "about.h"

// KDE
#include <KLocale>


// Program
static const char ProgramId[] =          "okteta";
static const char ProgramVersion[] =     "0.0.1";
// Author
static const char FWHKEmailAddress[] =   "kossebau@kde.org";


OktetaAboutData::OktetaAboutData()
: KAboutData( ProgramId, 0,
              ki18n("Okteta"), ProgramVersion, // name
              ki18n("KDE bytes editor"), // description
              KAboutData::License_GPL_V2,
              ki18n("Copyright 2007 Friedrich W. H. Kossebau"), //copyright
              ki18n("Edit files as streams of bytes"), // comment
              0, FWHKEmailAddress )
{
    setOrganizationDomain( "kde.org" );
    addAuthor( ki18n("Friedrich W. H. Kossebau"), // name
               ki18n("Author"), // task
               FWHKEmailAddress );
}

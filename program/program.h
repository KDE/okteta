/*
    This file is part of the Okteta program, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETAPROGRAM_H
#define OKTETAPROGRAM_H


// program
#include "about.h"

class KDocumentManager;
class KViewManager;


class OktetaProgram
{
public:
    OktetaProgram( int argc, char *argv[] );
    ~OktetaProgram();

public:
    int execute();
    void quit();

public:
    KDocumentManager *documentManager();
    KViewManager *viewManager();

protected:
    OktetaAboutData mAboutData;

    KDocumentManager *mDocumentManager;
    KViewManager *mViewManager;
};


inline KDocumentManager* OktetaProgram::documentManager() { return mDocumentManager; }
inline KViewManager* OktetaProgram::viewManager()         { return mViewManager; }

#endif

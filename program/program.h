/*
    This file is part of the Okteta program, part of the KDE project.
    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301  USA
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

/*
    This file is part of the Okteta program, made within the KDE community.

    Copyright 2006-2007,2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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


namespace Kasten2
{

class DialogHandler;
class DocumentManager;
class ViewManager;
class MultiDocumentStrategy;
// tmp
class ByteArrayViewProfileManager;


class OktetaProgram
{
  public:
    OktetaProgram( int argc, char* argv[] );
    ~OktetaProgram();

  public:
    int execute();
    void quit();

  public:
    DocumentManager* documentManager();
    ViewManager* viewManager();
    MultiDocumentStrategy* documentStrategy();
    ByteArrayViewProfileManager* byteArrayViewProfileManager();

  protected:
    OktetaAboutData mAboutData;

    DocumentManager* mDocumentManager;
    ViewManager* mViewManager;
    MultiDocumentStrategy* mDocumentStrategy;

    DialogHandler* mDialogHandler;

    ByteArrayViewProfileManager* mByteArrayViewProfileManager;
};


inline DocumentManager* OktetaProgram::documentManager() { return mDocumentManager; }
inline ViewManager* OktetaProgram::viewManager()         { return mViewManager; }
inline MultiDocumentStrategy* OktetaProgram::documentStrategy() { return mDocumentStrategy; }
inline ByteArrayViewProfileManager* OktetaProgram::byteArrayViewProfileManager() { return mByteArrayViewProfileManager; }

}

#endif

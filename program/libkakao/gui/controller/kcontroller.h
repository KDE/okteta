/***************************************************************************
                          kcontroller.h  -  description
                             -------------------
    begin                : Fri Jun 2 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KCONTROLLER_H
#define KCONTROLLER_H


// Qt
#include <QtCore/QObject>

class KXMLGuiWindow;

/* there are controllers for 
* (focused) documents
* document collections (projects)
  * static (related (or dependant) data)
  * dynamic (session)
* views
* view collections, like tabbed view or mdi
* program
* ...
Controllers can plug into the 
*/
class KController : public QObject
{
  public:
//    void plug( KXMLGuiWindow *window );
};

#endif

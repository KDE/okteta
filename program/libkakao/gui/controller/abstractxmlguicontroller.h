/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2006,2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ABSTRACTXMLGUICONTROLLER_H
#define ABSTRACTXMLGUICONTROLLER_H

// Qt
#include <QtCore/QObject>

class AbstractModel;
class KXMLGUIClient;

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
class AbstractXmlGuiController : public QObject
{
  Q_OBJECT

  public: // API to be implemented
    virtual void setTargetModel( AbstractModel* model ) {Q_UNUSED(model)}//= 0;
//    void plug( KXMLGUIClient* guiClient );
};

#endif

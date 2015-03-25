/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PRINTCONTROLLER_H
#define PRINTCONTROLLER_H

// lib
#include <kasten/okteta/oktetakastencontrollers_export.h>
// Kasten gui
#include <kasten/abstractxmlguicontroller.h>

class QAction;
class KXMLGUIClient;


namespace Kasten
{

class PrintTool;


class OKTETAKASTENCONTROLLERS_EXPORT PrintController : public AbstractXmlGuiController
{
  Q_OBJECT

  public:
    explicit PrintController( KXMLGUIClient* guiClient );
    virtual ~PrintController();

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  public: // TODO: this is a hack to enable the browser extension of the KPart to execute printing, rethink it
    void print();


  private:
    QAction *mPrintAction;

    PrintTool *mPrintTool;
};

}

#endif

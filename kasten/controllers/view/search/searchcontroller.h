/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

// lib
#include "oktetakastencontrollers_export.h"
// controller
#include "searchuserqueryable.h"
// libfinddialog
#include <kfinddirection.h>
// Kasten gui
#include <abstractxmlguicontroller.h>

class KXMLGUIClient;
class KAction;
class QWidget;


namespace Kasten2
{

class KSearchDialog;
class SearchTool;


class OKTETAKASTENCONTROLLERS_EXPORT SearchController : public AbstractXmlGuiController, public If::SearchUserQueryable
{
  Q_OBJECT

  public:
    SearchController( KXMLGUIClient* guiClient, QWidget* parentWidget );
    virtual ~SearchController();

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  public: // SearchUserQueryable API
    virtual bool queryContinue( KFindDirection direction ) const;

  private:
    void showDialog( KFindDirection Direction );

  private Q_SLOTS: // action slots
    void find();
    void findNext();
    void findPrevious();

    void onDataNotFound();

  private:
    QWidget* mParentWidget;

    KAction* mFindAction;
    KAction* mFindNextAction;
    KAction* mFindPrevAction;

    KSearchDialog* mSearchDialog;
    SearchTool* mTool;
};

}

#endif

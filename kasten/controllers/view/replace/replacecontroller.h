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

#ifndef REPLACECONTROLLER_H
#define REPLACECONTROLLER_H

// lib
#include "oktetakastencontrollers_export.h"
// controller
#include "replaceuserqueryable.h"
// lib
#include <kfinddirection.h>
// Kasten gui
#include <abstractxmlguicontroller.h>

class KXMLGUIClient;
class KAction;
class QWidget;


namespace Kasten2
{

class KReplaceDialog;
class KReplacePrompt;
class ReplaceTool;


class OKTETAKASTENCONTROLLERS_EXPORT ReplaceController : public AbstractXmlGuiController, public If::ReplaceUserQueryable
{
  Q_OBJECT

  public:
    ReplaceController( KXMLGUIClient* guiClient, QWidget* parentWidget );

    virtual ~ReplaceController();

  public: // AbstractXmlGuiController API
    virtual void setTargetModel( AbstractModel* model );

  public: // If::ReplaceUserQueryable API
    virtual bool queryContinue( KFindDirection direction, int noOfReplacements ) const;
    virtual ReplaceBehaviour queryReplaceCurrent() const;

  private Q_SLOTS: // action slots
    void replace();

    void onFinished( bool previousFound, int noOfReplacements );

  private:
    QWidget* mParentWidget;

    KAction *mReplaceAction;

    KReplaceDialog *mReplaceDialog;
    mutable KReplacePrompt* mReplacePrompt;
    ReplaceTool* mTool;
};

}

#endif

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006, 2008, 2010, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_ABSTRACTXMLGUICONTROLLER_HPP
#define KASTEN_ABSTRACTXMLGUICONTROLLER_HPP

// lib
#include "kastengui_export.hpp"
// Qt
#include <QObject>

class KXMLGUIClient;

namespace Kasten {

class AbstractModel;

/* there are controllers for 
* (focused) documents
* document collections (projects)
  * static (related (or dependent) data)
  * dynamic (session)
* views
* view collections, like tabbed view or mdi
* program
* ...
Controllers can plug into the 
*/
class KASTENGUI_EXPORT AbstractXmlGuiController : public QObject
{
    Q_OBJECT

protected:
    AbstractXmlGuiController();

public:
    ~AbstractXmlGuiController() override;

public: // API to be implemented
//    void plug( KXMLGUIClient* guiClient );
    virtual void setTargetModel(AbstractModel* model);   // = 0;

private:
    const QScopedPointer<class AbstractXmlGuiControllerPrivate> d_ptr;
};

}

#endif

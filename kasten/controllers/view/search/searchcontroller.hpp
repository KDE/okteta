/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_SEARCHCONTROLLER_HPP
#define KASTEN_SEARCHCONTROLLER_HPP

// controller
#include "searchuserqueryable.hpp"
// libfinddialog
#include "finddirection.hpp"
// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Std
#include <memory>

class KXMLGUIClient;
class QAction;
class QWidget;

namespace Kasten {

class AbstractUserMessagesHandler;
class SearchDialog;
class SearchTool;

class SearchController : public AbstractXmlGuiController
                       , public If::SearchUserQueryable
{
    Q_OBJECT

public:
    SearchController(KXMLGUIClient* guiClient, AbstractUserMessagesHandler* userMessagesHandler, QWidget* parentWidget);
    ~SearchController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

public: // SearchUserQueryable API
    [[nodiscard]]
    bool queryContinue(FindDirection direction) const override;

private:
    void showDialog(FindDirection Direction);

private Q_SLOTS: // action slots
    void find();
    void findNext();
    void findPrevious();

    void onDataNotFound();

private:
    AbstractUserMessagesHandler* const m_userMessagesHandler;
    QWidget* const mParentWidget;

    QAction* mFindAction;
    QAction* mFindNextAction;
    QAction* mFindPrevAction;

    // in (reverse) order of destruction
    std::unique_ptr<SearchTool> mTool;
    std::unique_ptr<SearchDialog> mSearchDialog;
};

}

#endif

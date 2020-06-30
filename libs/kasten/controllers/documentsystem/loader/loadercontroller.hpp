/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2008, 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_LOADERCONTROLLER_HPP
#define KASTEN_LOADERCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Qt
#include <QList>
#include <QUrl>

class KRecentFilesAction;
class KXMLGUIClient;
class QUrl;

namespace Kasten {

class AbstractDocumentStrategy;

class LoaderController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    LoaderController(AbstractDocumentStrategy* documentStrategy,
                     KXMLGUIClient* guiClient);
    ~LoaderController() override;

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void load();
    void loadRecent(const QUrl& url);
    void loadUrls(const QList<QUrl>& urls);

    void onUrlUsed(const QUrl& url);

private:
    AbstractDocumentStrategy* mDocumentStrategy;

    KRecentFilesAction* mOpenRecentAction;
};

}

#endif

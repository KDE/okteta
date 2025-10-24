/*
    SPDX-FileCopyrightText: 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef OKTETAMAINWINDOW_HPP
#define OKTETAMAINWINDOW_HPP

// Kasten gui
#include <Kasten/ShellWindow>

template <class T> class QList;
class QMimeData;

namespace Kasten {

class OktetaProgram;
class AbstractToolViewFactory;
class AbstractToolFactory;
class AbstractXmlGuiControllerFactory;

class OktetaMainWindow : public ShellWindow
{
    Q_OBJECT

public:
    explicit OktetaMainWindow(OktetaProgram* program);
    ~OktetaMainWindow() override;

protected: // KMainWindow API
    bool queryClose() override;

    void saveProperties(KConfigGroup& configGroup) override;
    void readProperties(const KConfigGroup& configGroup) override;

private:
    void setupControllers();
    void addToolFromFactory(const AbstractToolViewFactory& toolViewFactory,
                            const AbstractToolFactory& toolFactory);
    void addXmlGuiControllerFromFactory(const AbstractXmlGuiControllerFactory& factory);

private Q_SLOTS:
    void onDataOffered(const QMimeData* mimeData, bool& accept);
    void onDataDropped(const QMimeData* mimeData);
    void onCloseRequest(const QList<Kasten::AbstractView*>& views);
    void onNewDocumentRequested();

private:
    OktetaProgram* const mProgram;
};

}

#endif

/*
    This file is part of the Okteta program, made within the KDE community.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef OKTETAMAINWINDOW_HPP
#define OKTETAMAINWINDOW_HPP

// Kasten gui
#include <Kasten/ShellWindow>

template <class T> class QVector;
class QMimeData;

namespace Kasten {

class OktetaProgram;
class DocumentManager;
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

protected:
    void setupControllers();
    void addToolFromFactory(const AbstractToolViewFactory& toolViewFactory,
                            const AbstractToolFactory& toolFactory);
    void addXmlGuiControllerFromFactory(const AbstractXmlGuiControllerFactory& factory);

protected Q_SLOTS:
    void onDataOffered(const QMimeData* mimeData, bool& accept);
    void onDataDropped(const QMimeData* mimeData);
    void onCloseRequest(const QVector<Kasten::AbstractView*>& views);

protected:
    OktetaProgram* mProgram;
};

}

#endif

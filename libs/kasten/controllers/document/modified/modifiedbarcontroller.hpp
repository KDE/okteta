/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTEN_MODIFIEDBARCONTROLLER_HPP
#define KASTEN_MODIFIEDBARCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Kasten core
#include <Kasten/KastenCore>

class QLabel;

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocument;
class StatusBar;

class ModifiedBarController : public AbstractXmlGuiController
{
    Q_OBJECT

public:
    explicit ModifiedBarController(StatusBar* statusBar);

public: // AbstractXmlGuiController API
    void setTargetModel(AbstractModel* model) override;

private Q_SLOTS:
    void onContentFlagsChanged(Kasten::ContentFlags contentFlags);
    void onLocalSyncStateChanged(Kasten::LocalSyncState localSyncState);
    void onRemoteSyncStateChanged(Kasten::RemoteSyncState remoteSyncState);
    void onSynchronizerChanged(Kasten::AbstractModelSynchronizer* newSynchronizer);
    void onSynchronizerDeleted(QObject* synchronizer);

private:
    AbstractDocument* mDocument = nullptr;
    AbstractModelSynchronizer* mSynchronizer = nullptr;

    QLabel* mLocalStateLabel;
    QLabel* mRemoteStateLabel;
};

}

#endif

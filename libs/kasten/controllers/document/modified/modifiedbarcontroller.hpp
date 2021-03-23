/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009-2010 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_MODIFIEDBARCONTROLLER_HPP
#define KASTEN_MODIFIEDBARCONTROLLER_HPP

// Kasten gui
#include <Kasten/AbstractXmlGuiController>
// Kasten core
#include <Kasten/KastenCore>

namespace Kasten {
class AbstractModelSynchronizer;
class AbstractDocument;
class StatusBar;
class IconLabel;

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

    IconLabel* mLocalStateLabel;
    IconLabel* mRemoteStateLabel;
};

}

#endif

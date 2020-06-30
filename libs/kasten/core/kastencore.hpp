/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_KASTENCORE_HPP
#define KASTEN_KASTENCORE_HPP

// Qt core
#include <QFlags>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LOG_KASTEN_CORE)

namespace Kasten {

// TODO: reuse terms from vcs
enum LocalSyncState
{
    LocalInSync = 0, // TODO: find better name
    LocalHasChanges
};
// TODO: where to store access rights to remote?
enum RemoteSyncState
{
    RemoteInSync = 0, // TODO: find better name
    RemoteHasChanges,
    RemoteDeleted,
    // TODO: which KIO slaves are not supported by kdirwatch?
    RemoteUnknown,
    /// unknown, e.g. because connection not available/lost
    RemoteUnreachable
};

enum ContentFlag
{
    ContentStateNormal = 0, // TODO: is "normal" a good description?
    ContentHasUnstoredChanges = 1 << 0 // TODO: find better term for "changes not made persistent"
};
Q_DECLARE_FLAGS(ContentFlags, ContentFlag)
Q_DECLARE_OPERATORS_FOR_FLAGS(ContentFlags)

enum Answer
{
    Cancel = 0,
    Ok,
    Yes,
    No,
    Continue,
    PreviousQuestion,
    NextQuestion,
    Save,
    Overwrite,
    Discard,
    AdaptSize
};

}

#endif

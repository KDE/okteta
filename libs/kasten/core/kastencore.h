/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KASTENCORE_H
#define KASTENCORE_H

namespace Kasten
{
    //TODO: reuse terms from vcs
    enum LocalSyncState
    {
        LocalInSync = 0, //TODO: find better name
        LocalHasChanges
    };
    //TODO: where to store access rights to remote?
    enum RemoteSyncState
    {
        RemoteInSync = 0, //TODO: find better name
        RemoteHasChanges,
        RemoteNotSet,
        RemoteDeleted,
        // TODO: which KIO slaves are not supported by kdirwatch?
        RemoteUnknown,
        /// unknown, e.g. because connection not available/lost
        RemoteUnreachable
    };

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
    };
}

#endif

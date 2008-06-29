/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef KABSTRACTDOCUMENTFILESYSTEMSYNCHRONIZER_H
#define KABSTRACTDOCUMENTFILESYSTEMSYNCHRONIZER_H

// lib
#include "kabstractdocumentsynchronizer.h"

class KAbstractDocumentFileSystemSynchronizer : public KAbstractDocumentSynchronizer
{
  Q_OBJECT

  friend class AbstractFileSystemLoadJob;
  friend class AbstractFileSystemConnectJob;
  friend class AbstractFileSystemSyncWithRemoteJob;

  public:
    KAbstractDocumentFileSystemSynchronizer();
    virtual ~KAbstractDocumentFileSystemSynchronizer();

// TODO: virtual void onFileModified( FileChange );
// enum FileChange { FileDirty, FileCreated, FileDeleted );
  protected Q_SLOTS:
    void onFileDirty( const QString &filePath );
    void onFileCreated( const QString &filePath );
    void onFileDeleted( const QString &filePath );

  protected:
    class Private;
    Private * const d;
};

#endif

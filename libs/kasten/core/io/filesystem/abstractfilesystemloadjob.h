/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef ABSTRACTFILESYSTEMLOADJOB_H
#define ABSTRACTFILESYSTEMLOADJOB_H

// library
#include <abstractloadjob.h>

class KUrl;
class QWidget;

namespace Kasten
{

class AbstractModelFileSystemSynchronizer;


class AbstractFileSystemLoadJob : public AbstractLoadJob
{
  Q_OBJECT

  public:
    AbstractFileSystemLoadJob( AbstractModelFileSystemSynchronizer* synchronizer, const KUrl &url );
    virtual ~AbstractFileSystemLoadJob();

  public: // KJob API
    virtual void start();

  protected: // AbstractLoadJob API
    virtual void setDocument( KAbstractDocument *document );

  protected: // API to be implemented
    virtual void startLoadFromFile() = 0;

  protected Q_SLOTS:
    void load();

  protected:
    AbstractModelFileSystemSynchronizer* synchronizer() const;
    QString workFilePath() const;
    QWidget *widget() const;

  protected:
    class Private;
    Private * const d;
};

}

#endif

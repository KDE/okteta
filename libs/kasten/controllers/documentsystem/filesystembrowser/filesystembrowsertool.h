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

#ifndef FILESYSTEMBROWSERTOOL_H
#define FILESYSTEMBROWSERTOOL_H

// Kasten core
#include <abstracttool.h>

class KUrl;


namespace Kasten
{

class KDocumentManager;
class KAbstractDocument;


class FileSystemBrowserTool : public AbstractTool
{
  Q_OBJECT

  public:
    explicit FileSystemBrowserTool( KDocumentManager* documentManager );
    virtual ~FileSystemBrowserTool();

  public:
    void open( const KUrl& url );

  public:
    KUrl currentUrl() const;
    bool hasCurrentUrl() const;

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  Q_SIGNALS:
    void hasCurrentUrlChanged( bool hasCurrentUrl );

  protected: // sources
    KAbstractDocument* mDocument;

    KDocumentManager* mDocumentManager;
};

}

#endif

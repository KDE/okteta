/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "abstractmodelfilesystemexporter.h"

// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
#include <KMessageBox>

class AbstractModelFileSystemExporter::Private
{
  public:
    Private();
  public:
    QWidget *widget() const;
    void setWidget( QWidget *widget );
  protected:
    QWidget *mWidget;
};
inline AbstractModelFileSystemExporter::Private::Private() : mWidget( 0 ) {}
inline QWidget *AbstractModelFileSystemExporter::Private::widget() const { return mWidget; }
inline void AbstractModelFileSystemExporter::Private::setWidget( QWidget *widget) { mWidget = widget; }


AbstractModelFileSystemExporter::AbstractModelFileSystemExporter( const QString &remoteTypeName,
                                                                  const QString &remoteMimeType )
: AbstractModelExporter( remoteTypeName, remoteMimeType ),
  d( new Private() )
{
}

bool AbstractModelFileSystemExporter::doExport( AbstractModel *model, const AbstractModelSelection *selection,
                                                const KUrl &url )
{
    bool result;

    QString workFileName;
    KTemporaryFile temporaryFile;

    if( url.isLocalFile() )
        workFileName = url.path();
    else
    {
        temporaryFile.open();
        workFileName = temporaryFile.fileName();
    }

    if( writeToFile(model,selection,workFileName) )
    {
        if( !url.isLocalFile() )
        {
            result = KIO::NetAccess::upload( workFileName, url, d->widget() );
            if( !result )
                KMessageBox::error( d->widget(), KIO::NetAccess::lastErrorString() );
        }
        else
            result = true;
    }
    else
    {
        result = false;
        KMessageBox::error( d->widget(), i18nc("@info","Problem when saving to local filesystem.") );
    }

    return result;
}


AbstractModelFileSystemExporter::~AbstractModelFileSystemExporter()
{
    delete d;
}

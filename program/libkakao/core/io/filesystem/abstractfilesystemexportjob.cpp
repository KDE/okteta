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

#include "abstractfilesystemexportjob.h"

// KDE
#include <KIO/NetAccess>
#include <KTemporaryFile>
#include <KLocale>
// Qt
#include <QtCore/QTimer>


class AbstractFileSystemExportJob::Private
{
  public:
    Private( AbstractModel *model, const AbstractModelSelection *selection, const KUrl &url );
  public:
    void prepareWorkFile();
    void removeWorkFile();
  public:
    AbstractModel *model() const;
    const AbstractModelSelection *selection() const;
    const KUrl &url() const;
    QString workFilePath() const;
    QWidget *widget() const;
  protected:
    AbstractModel *mModel;
    const AbstractModelSelection *mSelection;
    const KUrl mUrl;
    KTemporaryFile *mTemporaryFile;
    QString mWorkFilePath;
    QWidget *mWidget;
};

inline AbstractFileSystemExportJob::Private::Private( AbstractModel *model, const AbstractModelSelection *selection,
                                 const KUrl &url)
 : mModel( model ), mSelection( selection ), mUrl( url ), mTemporaryFile( 0 ), mWidget( 0 )
{}
inline AbstractModel *AbstractFileSystemExportJob::Private::model()                    const { return mModel; }
inline const AbstractModelSelection *AbstractFileSystemExportJob::Private::selection() const { return mSelection; }
inline const KUrl &AbstractFileSystemExportJob::Private::url()                         const { return mUrl; }
inline QString AbstractFileSystemExportJob::Private::workFilePath()                    const { return mWorkFilePath; }
inline QWidget *AbstractFileSystemExportJob::Private::widget()                         const { return mWidget; }

inline void AbstractFileSystemExportJob::Private::prepareWorkFile()
{
    if( mUrl.isLocalFile() )
        mWorkFilePath = mUrl.path();
    else
    {
        mTemporaryFile = new KTemporaryFile;
        mTemporaryFile->open();
        mWorkFilePath = mTemporaryFile->fileName();
    }
}
inline void AbstractFileSystemExportJob::Private::removeWorkFile()
{
    delete mTemporaryFile;
}


AbstractFileSystemExportJob::AbstractFileSystemExportJob( AbstractModel *model, const AbstractModelSelection *selection,
                                 const KUrl &url )
: d( new Private(model,selection,url) )
{
}

AbstractModel *AbstractFileSystemExportJob::model() const { return d->model(); }
const AbstractModelSelection *AbstractFileSystemExportJob::selection() const { return d->selection(); }
QString AbstractFileSystemExportJob::workFilePath() const { return d->workFilePath(); }
QWidget *AbstractFileSystemExportJob::widget() const { return d->widget(); }


void AbstractFileSystemExportJob::start()
{
    QTimer::singleShot( 0, this, SLOT(exportToFile()) );
}

void AbstractFileSystemExportJob::exportToFile()
{
    d->prepareWorkFile();

    startExportToFile();
}

void AbstractFileSystemExportJob::completeExport( bool success )
{
    if( success )
    {
        if( !d->url().isLocalFile() )
        {
            success = KIO::NetAccess::upload( d->workFilePath(), d->url(), d->widget() );
            if( !success )
            {
                setError( KilledJobError );
                setErrorText( KIO::NetAccess::lastErrorString() );
            }
        }
    }
    else
    {
        setError( KilledJobError );
        setErrorText( i18nc("@info","Problem while saving to local filesystem.") );
    }

    d->removeWorkFile();

    emitResult();
}


AbstractFileSystemExportJob::~AbstractFileSystemExportJob()
{
    delete d;
}

#include "abstractfilesystemexportjob.moc"

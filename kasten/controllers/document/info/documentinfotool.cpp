/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008,2010,2012 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentinfotool.h"

// lib
#include "bytearraymodeliodevice.h"
#include <bytearraydocument.h>
// Kasten core
#include <documentsyncmanager.h>
#include <abstractmodelsynchronizer.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
#include <KUrl>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QTimer>


namespace Kasten2
{
static const int mimeTypeUpdateTimeInterval = 500; // msec


DocumentInfoTool::DocumentInfoTool( DocumentSyncManager* syncManager )
  : mDocument( 0 ),
    mByteArrayModel( 0 ),
    mSynchronizer( 0 ),
    mDocumentSyncManager( syncManager ),
    mMimeTypeUpdateTimer( new QTimer(this) ),
    mMimeType( 0 )
{
    setObjectName( QLatin1String( "DocumentInfo" ) );

    mMimeTypeUpdateTimer->setInterval( mimeTypeUpdateTimeInterval );
    mMimeTypeUpdateTimer->setSingleShot( true );
    connect( mMimeTypeUpdateTimer, SIGNAL(timeout()), SLOT(updateMimeType()) );
}

//TODO: file or document or ...?
QString DocumentInfoTool::title() const { return i18nc("@title:window", "File Info"); }
QString DocumentInfoTool::documentTitle() const
{
    return mDocument ? mDocument->title() : QString();
}

QString DocumentInfoTool::location() const
{
    QString result;
    if( mDocument )
    {
        const KUrl url = mDocumentSyncManager->urlOf( mDocument );
        result = url.isLocalFile() ? url.path() : url.prettyUrl();
    }
    return result;
}

int DocumentInfoTool::documentSize() const
{
    int documentSize = -1;
    if( mByteArrayModel )
        documentSize = mByteArrayModel->size();

    return documentSize;
}

void DocumentInfoTool::setTargetModel( AbstractModel* model )
{
    if( mSynchronizer ) mSynchronizer->disconnect( this );
    if( mDocument ) mDocument->disconnect( this );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

    mDocument = model ? model->findBaseModel<ByteArrayDocument*>() : 0;
    mByteArrayModel = mDocument ? mDocument->content() : 0;

    const bool hasDocument = ( mDocument != 0 );
    AbstractModelSynchronizer* synchronizer = 0;
    QString documentTitle;
    int documentSize = -1;
    if( hasDocument )
    {
        documentTitle = mDocument->title();
        documentSize = mByteArrayModel->size();
        synchronizer = mDocument->synchronizer();

        connect( mDocument, SIGNAL(titleChanged(QString)),
                 SIGNAL(documentTitleChanged(QString)) );
        connect( mDocument, SIGNAL(synchronizerChanged(Kasten2::AbstractModelSynchronizer*)),
                 SLOT(onSynchronizerChanged(Kasten2::AbstractModelSynchronizer*)) );
        connect( mByteArrayModel, SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                 SLOT(onContentsChanged()) );
    }

    onSynchronizerChanged( synchronizer );

    emit documentTitleChanged( documentTitle );
    emit documentSizeChanged( documentSize );
}

// TODO: should this be done in a worker thread, to not block the UI?
void DocumentInfoTool::updateMimeType()
{
    KMimeType::Ptr currentMimeType;

    if( mDocument )
    {
        // TODO: also get file mode, if available, for findByNameAndContent()
        const QString filename = mDocumentSyncManager->urlOf( mDocument ).fileName();

        Okteta::ByteArrayModelIoDevice byteArrayModelIoDevice( mByteArrayModel );
        currentMimeType = filename.isEmpty() ?
            KMimeType::findByContent( &byteArrayModelIoDevice ) :
            KMimeType::findByNameAndContent( filename, &byteArrayModelIoDevice );
    }

// TODO: KMimeType has no operator !=
//     if( *mMimeType != *currentMimeType )
    {
        mMimeType = currentMimeType;
        emit documentMimeTypeChanged( currentMimeType );
    }
}


void DocumentInfoTool::onContentsChanged()
{
    if( ! mMimeTypeUpdateTimer->isActive() )
        mMimeTypeUpdateTimer->start();

    emit documentSizeChanged( mByteArrayModel->size() );
}

void DocumentInfoTool::onSynchronizerChanged( AbstractModelSynchronizer* synchronizer )
{
    if( mSynchronizer ) mSynchronizer->disconnect( this );
    mSynchronizer = synchronizer;

    if( mSynchronizer )
    {
        connect( mSynchronizer, SIGNAL(urlChanged(KUrl)),
                 SLOT(onUrlChanged(KUrl)) );
    }
    emit locationChanged( location() );
}

void DocumentInfoTool::onUrlChanged( const KUrl& url )
{
    Q_UNUSED( url );

    emit locationChanged( location() );
}

DocumentInfoTool::~DocumentInfoTool() {}

}

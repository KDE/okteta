/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "documentinfotool.h"

// lib
#include <kbytearraydocument.h>
// Kakao core
#include <kdocumentsyncmanager.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
#include <KUrl>
// Qt
#include <QtGui/QApplication>


DocumentInfoTool::DocumentInfoTool( KDocumentSyncManager* syncManager )
 : mDocument( 0 ), mByteArrayModel( 0 ),
    mDocumentSyncManager( syncManager )
{
    setObjectName( "DocumentInfo" );
}

//TODO: file or document or ...?
QString DocumentInfoTool::title() const { return i18nc("@title:window", "File Info"); }
QString DocumentInfoTool::documentTitle() const
{
    return mDocument ? mDocument->title() : QString();
}
KMimeType::Ptr DocumentInfoTool::mimeType() const
{
    KMimeType::Ptr result;
    if( mDocument )
    {
#if 0
we have to create a QIODevice for AbstractByteArrayModel and 
do delayed tests for the mimetype on changes and cache the result.

While KMimeType does not support QIODevices this code can be reused:
KMimeType::Ptr KMimeType::findByContent( const QByteArray &data, int *accuracy )
{
    QBuffer buffer(const_cast<QByteArray *>(&data));
    buffer.open(QIODevice::ReadOnly);
    QByteArray cache;
    return KMimeTypeFactory::self()->findFromContent(
        &buffer, KMimeTypeFactory::AllRules, accuracy, cache );
}
#endif

        const KUrl url = mDocumentSyncManager->urlOf( mDocument );
        result = KMimeType::findByUrl( url, 0, true );
    }
    return result;
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
    if( mDocument ) mDocument->disconnect( this );

    mDocument = model ? model->findBaseModel<KByteArrayDocument*>() : 0;
    mByteArrayModel = mDocument ? mDocument->content() : 0;

    const bool hasDocument = ( mDocument != 0 );
    QString documentTitle;
    int documentSize = -1;
    if( hasDocument )
    {
        documentTitle = mDocument->title();
        documentSize = mByteArrayModel->size();
        connect( mDocument, SIGNAL(titleChanged( const QString & )),
                 SIGNAL(documentTitleChanged( const QString & )) );
        connect( mByteArrayModel, SIGNAL(contentsChanged( const KHE::ArrayChangeMetricsList& )),
                 SLOT(onContentsChanged()) );
    }

    emit documentTitleChanged( documentTitle );
    emit documentMimeTypeChanged( mimeType() );
    emit locationChanged( location() );
    emit documentSizeChanged( documentSize );
}

void DocumentInfoTool::onContentsChanged()
{
    emit documentSizeChanged( mByteArrayModel->size() );
}

DocumentInfoTool::~DocumentInfoTool() {}

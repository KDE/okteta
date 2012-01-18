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

#ifndef DOCUMENTINFOTOOL_H
#define DOCUMENTINFOTOOL_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten core
#include <abstracttool.h>
// KDE
#include <KMimeType>

namespace Okteta {
class AbstractByteArrayModel;
}
class KUrl;
class QString;
class QTimer;


namespace Kasten2
{

class ByteArrayDocument;
class AbstractModelSynchronizer;
class DocumentSyncManager;


class OKTETAKASTENCONTROLLERS_EXPORT DocumentInfoTool : public AbstractTool
{
  Q_OBJECT

  public:
    explicit DocumentInfoTool( DocumentSyncManager* syncManager );
    virtual ~DocumentInfoTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public:
    QString documentTitle() const;
    KMimeType::Ptr mimeType() const;
    QString location() const;
    int documentSize() const;

  Q_SIGNALS:
    void documentTitleChanged( const QString& documentTitle );
    void documentMimeTypeChanged( KMimeType::Ptr mimeType );
    void locationChanged( const QString& location );
    void documentSizeChanged( int newSize );

  private Q_SLOTS:
    void updateMimeType();

    void onContentsChanged();
    void onSynchronizerChanged( Kasten2::AbstractModelSynchronizer* synchronizer );
    void onUrlChanged( const KUrl& url );

  private:
    ByteArrayDocument* mDocument;
    Okteta::AbstractByteArrayModel *mByteArrayModel;

    AbstractModelSynchronizer* mSynchronizer;

    DocumentSyncManager* mDocumentSyncManager;

    QTimer* mMimeTypeUpdateTimer;
    KMimeType::Ptr mMimeType;
};


inline KMimeType::Ptr DocumentInfoTool::mimeType() const { return mMimeType; }

}

#endif

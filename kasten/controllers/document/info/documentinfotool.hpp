/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTINFOTOOL_HPP
#define KASTEN_DOCUMENTINFOTOOL_HPP

// Kasten core
#include <Kasten/AbstractTool>
// Qt
#include <QMimeType>

namespace Okteta {
class AbstractByteArrayModel;
}
class QString;
class QTimer;

namespace Kasten {

class ByteArrayDocument;
class AbstractModelSynchronizer;
class DocumentSyncManager;

class DocumentInfoTool : public AbstractTool
{
    Q_OBJECT

public:
    explicit DocumentInfoTool(DocumentSyncManager* syncManager);
    DocumentInfoTool() = delete;

    ~DocumentInfoTool() override;

public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    QString title() const override;

    void setTargetModel(AbstractModel* model) override;

public:
    QString documentTitle() const;
    QMimeType mimeType() const;
    QString location() const;
    int documentSize() const;

Q_SIGNALS:
    void documentTitleChanged(const QString& documentTitle);
    void documentMimeTypeChanged(const QMimeType& mimeType);
    void locationChanged(const QString& location);
    void documentSizeChanged(int newSize);

private Q_SLOTS:
    void updateMimeType();

    void onContentsChanged();
    void onSynchronizerChanged(Kasten::AbstractModelSynchronizer* synchronizer);
    void onUrlChanged(const QUrl& url);

private:
    ByteArrayDocument* mDocument = nullptr;
    Okteta::AbstractByteArrayModel* mByteArrayModel = nullptr;

    AbstractModelSynchronizer* mSynchronizer = nullptr;

    DocumentSyncManager* mDocumentSyncManager;

    QTimer* mMimeTypeUpdateTimer;
    QMimeType mMimeType;
};

inline QMimeType DocumentInfoTool::mimeType() const { return mMimeType; }

}

#endif

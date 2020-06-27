/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2010, 2012 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "documentinfotool.hpp"

// lib
#include "bytearraymodeliodevice.hpp"
// Okteta Kasten core
#include <Kasten/Okteta/ByteArrayDocument>
// Kasten core
#include <Kasten/DocumentSyncManager>
#include <Kasten/AbstractModelSynchronizer>

// Okteta core
#include <Okteta/AbstractByteArrayModel>
#include <Okteta/ArrayChangeMetricsList>
// KF
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QTimer>
#include <QUrl>
#include <QMimeDatabase>

namespace Kasten {

static constexpr int mimeTypeUpdateTimeInterval = 500; // msec

DocumentInfoTool::DocumentInfoTool(DocumentSyncManager* syncManager)
    : mDocumentSyncManager(syncManager)
    , mMimeTypeUpdateTimer(new QTimer(this))
{
    setObjectName(QStringLiteral("DocumentInfo"));

    mMimeTypeUpdateTimer->setInterval(mimeTypeUpdateTimeInterval);
    mMimeTypeUpdateTimer->setSingleShot(true);
    connect(mMimeTypeUpdateTimer, &QTimer::timeout, this, &DocumentInfoTool::updateMimeType);
}

DocumentInfoTool::~DocumentInfoTool() = default;

// TODO: file or document or ...?
QString DocumentInfoTool::title() const { return i18nc("@title:window", "File Info"); }
QString DocumentInfoTool::documentTitle() const
{
    return mDocument ? mDocument->title() : QString();
}

QString DocumentInfoTool::location() const
{
    QString result;
    if (mDocument) {
        const QUrl url = mDocumentSyncManager->urlOf(mDocument);
        result = url.toDisplayString(QUrl::PrettyDecoded | QUrl::PreferLocalFile);
    }
    return result;
}

int DocumentInfoTool::documentSize() const
{
    int documentSize = -1;
    if (mByteArrayModel) {
        documentSize = mByteArrayModel->size();
    }

    return documentSize;
}

void DocumentInfoTool::setTargetModel(AbstractModel* model)
{
    if (mSynchronizer) {
        mSynchronizer->disconnect(this);
    }
    if (mDocument) {
        mDocument->disconnect(this);
    }
    if (mByteArrayModel) {
        mByteArrayModel->disconnect(this);
    }

    mDocument = model ? model->findBaseModel<ByteArrayDocument*>() : nullptr;
    mByteArrayModel = mDocument ? mDocument->content() : nullptr;

    const bool hasDocument = (mDocument != nullptr);
    AbstractModelSynchronizer* synchronizer = nullptr;
    QString documentTitle;
    int documentSize = -1;
    if (hasDocument) {
        documentTitle = mDocument->title();
        documentSize = mByteArrayModel->size();
        synchronizer = mDocument->synchronizer();

        connect(mDocument, &ByteArrayDocument::titleChanged,
                this, &DocumentInfoTool::documentTitleChanged);
        connect(mDocument, &ByteArrayDocument::synchronizerChanged,
                this, &DocumentInfoTool::onSynchronizerChanged);
        connect(mByteArrayModel, &Okteta::AbstractByteArrayModel::contentsChanged,
                this, &DocumentInfoTool::onContentsChanged);
    }

    onSynchronizerChanged(synchronizer);

    emit documentTitleChanged(documentTitle);
    emit documentSizeChanged(documentSize);
}

// TODO: should this be done in a worker thread, to not block the UI?
void DocumentInfoTool::updateMimeType()
{
    QMimeType currentMimeType;

    if (mDocument) {
        // TODO: also get file mode, if available, for findByNameAndContent()
        const QString filename = mDocumentSyncManager->urlOf(mDocument).fileName();

        Okteta::ByteArrayModelIoDevice byteArrayModelIoDevice(mByteArrayModel);
        QMimeDatabase db;
        currentMimeType = filename.isEmpty() ?
                          db.mimeTypeForData(&byteArrayModelIoDevice) :
                          db.mimeTypeForFileNameAndData(filename, &byteArrayModelIoDevice);
    }

    if (mMimeType != currentMimeType) {
        mMimeType = currentMimeType;
        emit documentMimeTypeChanged(currentMimeType);
    }
}

void DocumentInfoTool::onContentsChanged()
{
    if (!mMimeTypeUpdateTimer->isActive()) {
        mMimeTypeUpdateTimer->start();
    }

    emit documentSizeChanged(mByteArrayModel->size());
}

void DocumentInfoTool::onSynchronizerChanged(AbstractModelSynchronizer* synchronizer)
{
    // do an instant update, no need to delay
    if (mMimeTypeUpdateTimer->isActive()) {
        mMimeTypeUpdateTimer->stop();
    }
    updateMimeType();

    if (mSynchronizer) {
        mSynchronizer->disconnect(this);
    }
    mSynchronizer = synchronizer;

    if (mSynchronizer) {
        connect(mSynchronizer, &AbstractModelSynchronizer::urlChanged,
                this, &DocumentInfoTool::onUrlChanged);
    }

    emit locationChanged(location());
}

void DocumentInfoTool::onUrlChanged(const QUrl& url)
{
    Q_UNUSED(url);

    emit locationChanged(location());
}

}

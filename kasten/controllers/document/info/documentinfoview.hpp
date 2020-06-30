/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_DOCUMENTINFOVIEW_HPP
#define KASTEN_DOCUMENTINFOVIEW_HPP

// Qt
#include <QWidget>

class KSqueezedTextLabel;
class QString;
class QMimeType;
class QLabel;

namespace Kasten {

class DocumentInfoTool;

class DocumentInfoView : public QWidget
{
    Q_OBJECT

public:
    explicit DocumentInfoView(DocumentInfoTool* tool, QWidget* parent = nullptr);
    ~DocumentInfoView() override;

public:
    DocumentInfoTool* tool() const;

private Q_SLOTS:
    void onDocumentTitleChanged(const QString& documentTitle);
    void onMimeTypeChanged(const QMimeType& mimeType);
    void onLocationChanged(const QString& location);
    void onDocumentSizeChanged(int newSize);

private:
    DocumentInfoTool* mTool;

    QLabel* mIconLabel;
    QLabel* mDocumentTitleLabel;
    QLabel* mMimeTypeLabel;
    KSqueezedTextLabel* mLocationLabel;
    QLabel* mSizeLabel;
};

inline DocumentInfoTool* DocumentInfoView::tool() const { return mTool; }

}

#endif

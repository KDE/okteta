/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILESYSTEMBROWSERVIEW_HPP
#define KASTEN_FILESYSTEMBROWSERVIEW_HPP

// Qt
#include <QWidget>

class KDirOperator;
class KUrlNavigator;
class QUrl;
class KFileItem;
class KActionCollection;
class KToolBar;

namespace Kasten {

class FileSystemBrowserTool;

class FileSystemBrowserView : public QWidget
{
    Q_OBJECT

public:
    explicit FileSystemBrowserView(FileSystemBrowserTool* tool, QWidget* parent = nullptr);
    ~FileSystemBrowserView() override;

public:
    FileSystemBrowserTool* tool() const;

private Q_SLOTS:
    void init();

    void setDirOperatorUrl(const QUrl& url);
    void setNavigatorUrl(const QUrl& url);
    void syncCurrentDocumentDirectory();
    void openFile(const KFileItem& fileItem);

private:
    FileSystemBrowserTool* const mTool;

    KActionCollection* mActionCollection;
    KToolBar* mToolbar;
    KDirOperator* mDirOperator;
    KUrlNavigator* mUrlNavigator;
};

inline FileSystemBrowserTool* FileSystemBrowserView::tool() const { return mTool; }

}

#endif

/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_FILESYSTEMBROWSERTOOLVIEW_HPP
#define KASTEN_FILESYSTEMBROWSERTOOLVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolView>
// Std
#include <memory>

namespace Kasten {

class FileSystemBrowserView;
class FileSystemBrowserTool;

class FileSystemBrowserToolView : public AbstractToolView
{
    Q_OBJECT

public:
    explicit FileSystemBrowserToolView(FileSystemBrowserTool* tool);
    ~FileSystemBrowserToolView() override;

public: // AbstractToolView API
    [[nodiscard]]
    QWidget* widget() const override;
    [[nodiscard]]
    QString title() const override;
    [[nodiscard]]
    AbstractTool* tool() const override;

private:
    const std::unique_ptr<FileSystemBrowserView> mWidget;
};

}

#endif

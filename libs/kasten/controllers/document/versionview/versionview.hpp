/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_VERSIONVIEW_HPP
#define KASTEN_VERSIONVIEW_HPP

// Qt
#include <QWidget>

class QTreeView;

namespace Kasten {

class VersionTableModel;
class VersionViewTool;
class AbstractModel;

class VersionView : public QWidget
{
    Q_OBJECT

public:
    explicit VersionView(VersionViewTool* tool, QWidget* parent = nullptr);
    ~VersionView() override;

public:
    VersionViewTool* tool() const;

public Q_SLOTS:
    void setModel(Kasten::AbstractModel* model);

private:
    VersionViewTool* mTool;

    VersionTableModel* mVersionTableModel;

    QTreeView* mVersionTableView;
};

inline VersionViewTool* VersionView::tool() const { return mTool; }

}

#endif

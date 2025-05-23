/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INFOVIEW_HPP
#define KASTEN_INFOVIEW_HPP

// Qt
#include <QWidget>

class QLabel;
class QTreeView;
class QAction;

namespace Kasten {

class InfoTool;

class InfoView : public QWidget
{
    Q_OBJECT

public:
    explicit InfoView(InfoTool* tool, QWidget* parent = nullptr);
    ~InfoView() override;

public:
    [[nodiscard]]
    InfoTool* tool() const;

public Q_SLOTS:
    void updateHeader();
    void setByteArraySize(int size);

private Q_SLOTS:
    void resizeColumnsWidth();
    void onCustomContextMenuRequested(QPoint pos);
    void onCopyButtonClicked();
    void onTableSelectionChanged();

private:
    InfoTool* const mTool;

    QLabel* mSizeLabel;
    QTreeView* mStatisticTableView;
    QAction* mUpdateAction;
    QAction* mCopyAction;
};

inline InfoTool* InfoView::tool() const { return mTool; }

}

#endif

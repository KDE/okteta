/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2008-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_INFOVIEW_HPP
#define KASTEN_INFOVIEW_HPP

// Qt
#include <QWidget>

class QPushButton;
class QLabel;
class QTreeView;

namespace Kasten {

class InfoTool;

class InfoView : public QWidget
{
    Q_OBJECT

public:
    explicit InfoView(InfoTool* tool, QWidget* parent = nullptr);
    ~InfoView() override;

public:
    InfoTool* tool() const;

public Q_SLOTS:
    void updateHeader();
    void setByteArraySize(int size);

private Q_SLOTS:
    void setFixedFontByGlobalSettings();
    void resizeColumnsWidth();

private:
    InfoTool* mTool;

    QLabel* mSizeLabel;
    QTreeView* mStatisticTableView;
    QPushButton* mUpdateButton;
};

inline InfoTool* InfoView::tool() const { return mTool; }

}

#endif

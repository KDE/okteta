/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_BYTETABLEVIEW_HPP
#define KASTEN_BYTETABLEVIEW_HPP

// Kasten gui
#include <Kasten/AbstractToolWidget>

class QPushButton;
class QSpinBox;
class QTreeView;
class QModelIndex;

namespace Kasten {

class ByteTableTool;

class ByteTableView : public AbstractToolWidget
{
    Q_OBJECT

public:
    explicit ByteTableView(ByteTableTool* tool, QWidget* parent = nullptr);
    ~ByteTableView() override;

public:
    ByteTableTool* tool() const;

public Q_SLOTS:
    void onInsertClicked();
    void onDoubleClicked(const QModelIndex& index);

private Q_SLOTS:
    void setFixedFontByGlobalSettings();
    void resizeColumnsWidth();

private:
    ByteTableTool* mTool;

    QSpinBox* mInsertCountEdit;
    QTreeView* mByteTableView;
    QPushButton* mInsertButton;
};

inline ByteTableTool* ByteTableView::tool() const { return mTool; }

}

#endif

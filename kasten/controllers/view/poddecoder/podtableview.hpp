/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODTABLEVIEW_HPP
#define KASTEN_PODTABLEVIEW_HPP

// lib
#include "abstractdifferentsizedialog.hpp"
// Qt
#include <QWidget>

class KComboBox;
class QTreeView;
class QCheckBox;
class QModelIndex;

namespace Kasten {

class PODTableModel;
class PODDecoderTool;

class PODTableView : public QWidget
                   , public AbstractDifferentSizeDialog
{
    Q_OBJECT

public:
    explicit PODTableView(PODDecoderTool* tool, QWidget* parent = nullptr);
    ~PODTableView() override;

public:
    PODDecoderTool* tool() const;

public: // QObject API
    bool eventFilter(QObject* object, QEvent* event) override;

public: // AbstractDifferentSizeDialog API
    Answer query(int newValueSize, int oldValueSize, int sizeLeft) override;

private Q_SLOTS:
    void onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous);

private:
    PODDecoderTool* mTool;

    PODTableModel* mPODTableModel;

    QTreeView* mPODTableView;
    KComboBox* mByteOrderSelection;
    QCheckBox* mUnsignedAsHexCheck;

    QWidget* mPODTableViewFocusChild = nullptr;
};

inline PODDecoderTool* PODTableView::tool() const { return mTool; }

}

#endif

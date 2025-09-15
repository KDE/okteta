/*
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

class PODDelegate;
class PODTableModel;
class PODDecoderTool;
class AbstractUserMessagesHandler;

class PODTableView : public QWidget
                   , public AbstractDifferentSizeDialog
{
    Q_OBJECT

public:
    explicit PODTableView(PODDecoderTool* tool, AbstractUserMessagesHandler* userMessagesHandler,
                          QWidget* parent = nullptr);
    ~PODTableView() override;

public:
    [[nodiscard]]
    PODDecoderTool* tool() const;

public: // QObject API
    [[nodiscard]]
    bool eventFilter(QObject* object, QEvent* event) override;

public: // AbstractDifferentSizeDialog API
    [[nodiscard]]
    Answer query(int newValueSize, int oldValueSize, int sizeLeft) override;

private Q_SLOTS:
    void onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous);
    void onCustomContextMenuRequested(QPoint pos);
    void editData();
    void copyToClipboard();
    void selectBytesInView();

private:
    PODDecoderTool* const mTool;

    AbstractUserMessagesHandler* const m_userMessagesHandler;

    PODTableModel* mPODTableModel;

    QTreeView* mPODTableView;
    PODDelegate* mPODDelegate;
    KComboBox* mByteOrderSelection;
    QCheckBox* mUnsignedAsHexCheck;

    QWidget* mPODTableViewFocusChild = nullptr;
};

inline PODDecoderTool* PODTableView::tool() const { return mTool; }

}

#endif

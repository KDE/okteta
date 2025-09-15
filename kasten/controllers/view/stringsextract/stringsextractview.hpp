/*
    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTVIEW_HPP
#define KASTEN_STRINGSEXTRACTVIEW_HPP

// Qt
#include <QWidget>

class QTreeView;
class QSortFilterProxyModel;
class QLabel;
class QModelIndex;
class QSpinBox;
class QAction;

namespace Kasten {

class AbstractUserMessagesHandler;
class StringsExtractTool;
class ContainedStringTableModel;

class StringsExtractView : public QWidget
{
    Q_OBJECT

public:
    explicit StringsExtractView(StringsExtractTool* tool,
                                AbstractUserMessagesHandler* userMessagseHandler,
                                QWidget* parent = nullptr);
    ~StringsExtractView() override;

public:
    [[nodiscard]]
    StringsExtractTool* tool() const;

public: // QObject API
    [[nodiscard]]
    bool eventFilter(QObject* object, QEvent* event) override;

private Q_SLOTS: // gui
    void onStringDoubleClicked(const QModelIndex& index);
    void onCustomContextMenuRequested(QPoint pos);
    void onStringSelectionChanged();
    void onGotoButtonClicked();
    void onCopyButtonClicked();
    void onCopyOffsetTriggered();
    void onSelectStringTriggered();

private Q_SLOTS: // tool
    void onStringsUptodateChanged(bool stringsUptodate);
    void onApplyableChanged(bool isApplyable);
    void onExtractionDone(int extractedStringSCount);
    void onCanHighlightStringChanged(bool canHighlightString);

private:
    StringsExtractTool* const mTool;

    AbstractUserMessagesHandler* const m_userMessagesHandler;

    ContainedStringTableModel* mContainedStringTableModel;
    QSortFilterProxyModel* mSortFilterProxyModel;

    QTreeView* mContainedStringTableView;
    QLabel* m_emptyListOverlayLabel;
    QSpinBox* mMinLengthSpinBox;
    QAction* mUpdateAction;
    QAction* mCopyAction;
    QAction* mCopyOffsetAction;
    QAction* mGotoAction;
    QAction* mSelectAction;
};

inline StringsExtractTool* StringsExtractView::tool() const { return mTool; }

}

#endif

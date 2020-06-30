/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRINGSEXTRACTVIEW_HPP
#define KASTEN_STRINGSEXTRACTVIEW_HPP

// Qt
#include <QWidget>

class ContainedStringTableModel;

class QPushButton;

class QTreeView;
class QSortFilterProxyModel;
class QModelIndex;
class QSpinBox;

namespace Kasten {

class StringsExtractTool;

class StringsExtractView : public QWidget
{
    Q_OBJECT

public:
    explicit StringsExtractView(StringsExtractTool* tool, QWidget* parent = nullptr);
    ~StringsExtractView() override;

public:
    StringsExtractTool* tool() const;

public: // QObject API
    bool eventFilter(QObject* object, QEvent* event) override;

private Q_SLOTS: // gui
    void onStringDoubleClicked(const QModelIndex& index);
    void onStringSelectionChanged();
    void onGotoButtonClicked();
    void onCopyButtonClicked();

private Q_SLOTS: // tool
    void onStringsUptodateChanged(bool stringUptodate);
    void onApplyableChanged(bool isApplyable);
    void onCanHighlightStringChanged(bool isSelectable);

private Q_SLOTS:
    void setFixedFontByGlobalSettings();

private:
    StringsExtractTool* mTool;

    ContainedStringTableModel* mContainedStringTableModel;
    QSortFilterProxyModel* mSortFilterProxyModel;

    QTreeView* mContainedStringTableView;
    QSpinBox* mMinLengthSpinBox;
    QPushButton* mUpdateButton;
    QPushButton* mCopyButton;
    QPushButton* mGotoButton;
};

inline StringsExtractTool* StringsExtractView::tool() const { return mTool; }

}

#endif

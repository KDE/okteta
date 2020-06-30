/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREVIEW_HPP
#define KASTEN_STRUCTUREVIEW_HPP

#include <QWidget>
#include "structureviewitemdelegate.hpp"

namespace Okteta {
class AbstractByteArrayModel;
}

class QTreeView;
class QPushButton;

namespace Kasten {
class StructuresTool;
class StructureTreeModel;

class StructureView : public QWidget
{
    Q_OBJECT

public:
    explicit StructureView(StructuresTool* tool, QWidget* parent = nullptr);
    ~StructureView() override;

public:
    StructuresTool* tool() const;
    bool eventFilter(QObject* object, QEvent* event) override;

private Q_SLOTS:
    void openSettingsDlg();
    void openScriptConsole();
    void onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous);
    void onCursorIndexChange();
    void lockButtonToggled();
    void setLockButtonState(bool structureLocked);
    void onByteArrayModelChanged(Okteta::AbstractByteArrayModel* model);

private:
    void setLockButtonState(const QModelIndex& current);

private:
    StructuresTool* mTool;

    StructureTreeModel* mStructureTreeModel;

    StructureViewItemDelegate* mDelegate;

    QTreeView* mStructTreeView;

    QPushButton* mValidateButton;
    QPushButton* mSettingsButton;
    QPushButton* mScriptConsoleButton;
    QPushButton* mLockStructureButton;

    QWidget* mStructTreeViewFocusChild;
};

}

#endif

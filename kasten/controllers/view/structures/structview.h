/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2012 Alex Richardson <alex.richardson@gmx.de>
 *   Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef STRUCTVIEW_H_
#define STRUCTVIEW_H_
#include <QWidget>
#include "structviewitemdelegate.h"

namespace Okteta {
class AbstractByteArrayModel;
}

class KComboBox;
class QTreeView;
class QPushButton;
class KTextEdit;
//class ModelTest;

namespace Kasten
{
class StructTool;
class StructTreeModel;

class StructView: public QWidget
{
Q_OBJECT

public:
    explicit StructView(StructTool* tool, QWidget* parent = nullptr);
    virtual ~StructView();

public:
    StructTool* tool() const;
    virtual bool eventFilter(QObject* object, QEvent* event);

protected:
    StructTool* mTool;

    StructTreeModel* mStructTreeModel;
//    ModelTest* mModeltest;

    QTreeView* mStructTreeView;
    QPushButton* mValidateButton;
    QPushButton* mSettingsButton;
    QPushButton* mScriptConsoleButton;
    StructViewItemDelegate* mDelegate;
    QPushButton* mLockStructureButton;
    QWidget* mStructTreeViewFocusChild;

protected Q_SLOTS:
    void openSettingsDlg();
    void openScriptConsole();
    void onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous);
    void onCursorIndexChange();
    void lockButtonToggled();
    void setLockButtonState(bool structureLocked);
    void onByteArrayModelChanged(Okteta::AbstractByteArrayModel* model);
private:
    void setLockButtonState(const QModelIndex& current);
};

inline StructTool* StructView::tool() const
{
    return mTool;
}

}
#endif /* STRUCTVIEW_H_ */

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STRINGSEXTRACTVIEW_H
#define STRINGSEXTRACTVIEW_H

// Qt
#include <QtGui/QWidget>

class ContainedStringTableModel;

class KPushButton;

class QTreeView;
class QSortFilterProxyModel;
class QModelIndex;
class QSpinBox;


namespace Kasten2
{

class StringsExtractTool;


class StringsExtractView : public QWidget
{
  Q_OBJECT

  public:
    explicit StringsExtractView( StringsExtractTool *tool, QWidget* parent = 0 );
    virtual ~StringsExtractView();

  public:
    StringsExtractTool *tool() const;

  public: // QObject API
    virtual bool eventFilter( QObject* object, QEvent* event );

  private Q_SLOTS: // gui
    void onStringDoubleClicked( const QModelIndex &index );
    void onStringSelectionChanged();
    void onGotoButtonClicked();
    void onCopyButtonClicked();

  private Q_SLOTS: // tool
    void onStringsUptodateChanged( bool stringUptodate );
    void onApplyableChanged( bool isApplyable );
    void onCanHighlightStringChanged( bool isSelectable );

  private Q_SLOTS:
    void setFixedFontByGlobalSettings();

  private:
    StringsExtractTool *mTool;

    ContainedStringTableModel *mContainedStringTableModel;
    QSortFilterProxyModel *mSortFilterProxyModel;

    QTreeView *mContainedStringTableView;
    QSpinBox *mMinLengthSpinBox;
    KPushButton *mUpdateButton;
    KPushButton *mCopyButton;
    KPushButton *mGotoButton;
};


inline StringsExtractTool *StringsExtractView::tool() const { return mTool; }

}

#endif

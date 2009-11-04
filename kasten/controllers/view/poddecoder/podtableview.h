/*
    This file is part of the Okteta Kasten module, part of the KDE project.

    Copyright 2006-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PODTABLEVIEW_H
#define PODTABLEVIEW_H

// Qt
#include <QtGui/QWidget>

class KComboBox;
class QTreeView;
class QCheckBox;
class QModelIndex;


namespace Kasten
{

class PODTableModel;
class PODDecoderTool;


class PODTableView : public QWidget
{
  Q_OBJECT

  public:
    explicit PODTableView( PODDecoderTool* tool, QWidget* parent = 0 );
    virtual ~PODTableView();

  public:
    PODDecoderTool* tool() const;

  public: // QObject API
    virtual bool eventFilter( QObject* object, QEvent* event );

  protected Q_SLOTS:
    void onCurrentRowChanged( const QModelIndex& current, const QModelIndex& previous );

  protected:
    PODDecoderTool* mTool;

    PODTableModel* mPODTableModel;

    QTreeView* mPODTableView;
    KComboBox* mByteOrderSelection;
    QCheckBox* mUnsignedAsHexCheck;
};


inline PODDecoderTool* PODTableView::tool() const { return mTool; }

}

#endif

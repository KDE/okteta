/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef PODDELEGATE_H
#define PODDELEGATE_H

// Qt
#include <QtGui/QStyledItemDelegate>
#include <QtCore/QPointer>


namespace Kasten2
{
class PODDecoderTool;


class PODDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:
    explicit PODDelegate( PODDecoderTool* tool, QObject* parent = 0 );

    virtual ~PODDelegate();

  public: // QAbstractItemDelegate
//     virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
//     virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex& index ) const;
//     virtual void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    virtual void setEditorData( QWidget* editor, const QModelIndex& index ) const;
    virtual void setModelData( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;

  public: // QStyledItemDelegate API
    virtual QString displayText( const QVariant& value, const QLocale& locale ) const;

  private Q_SLOTS:
    void onEditorDone();
    void onReadOnlyChanged( bool isReadOnly ) const;

  private:
    PODDecoderTool* mTool;

    mutable QPointer<QWidget> mEditor;
};

}

#endif

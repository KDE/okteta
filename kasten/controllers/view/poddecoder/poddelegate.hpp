/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_PODDELEGATE_HPP
#define KASTEN_PODDELEGATE_HPP

// Qt
#include <QStyledItemDelegate>
#include <QPointer>

namespace Kasten {
class PODDecoderTool;

class PODDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit PODDelegate(PODDecoderTool* tool, QObject* parent = nullptr);

    ~PODDelegate() override;

public: // QAbstractItemDelegate
//     virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
//     virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex& index ) const;
//     virtual void updateEditorGeometry( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;

public: // QStyledItemDelegate API
    QString displayText(const QVariant& value, const QLocale& locale) const override;

private Q_SLOTS:
    void onEditorDone();
    void onReadOnlyChanged(bool isReadOnly) const;

private:
    PODDecoderTool* mTool;

    mutable QPointer<QWidget> mEditor = nullptr;
};

}

#endif

/*
    This file is part of the Okteta Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef STRUCTUREITEMDELEGATE_HPP
#define STRUCTUREITEMDELEGATE_HPP

// KF
#include <KWidgetItemDelegate>

class QCheckBox;
class QPushButton;

class StructureItemDelegate : public KWidgetItemDelegate
{
    Q_OBJECT

public:
    StructureItemDelegate(QAbstractItemView* itemView, QObject* parent);
    ~StructureItemDelegate() override;

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

protected:
    QList<QWidget*> createItemWidgets(const QModelIndex& index) const override;
    void updateItemWidgets(const QList<QWidget*> widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const override;

private Q_SLOTS:
    void handleAboutClicked();
    void handleCheckClicked(bool checked);

private:
    enum LayoutMode {
        LayoutForActualSize, ///< layout with rects for where objects are exeactly placed
        LayoutForSizeHint, ///< layout with sizes set to hinted ones, positions are ignored
    };
    void layoutItem(const QStyleOptionViewItem& option,
                    const QModelIndex& index,
                    QRect* checkBoxRect, QRect* iconRect, QRect* textRect, QRect* aboutButtonRect,
                    int* focusFrameHMarginToUpdate,
                    LayoutMode layoutMode) const;

private:
    std::unique_ptr<QCheckBox> m_sampleCheckBox;
    std::unique_ptr<QPushButton> m_sampleAboutButton;
};

#endif

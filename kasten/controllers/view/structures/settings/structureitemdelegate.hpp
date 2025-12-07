/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KASTEN_STRUCTUREITEMDELEGATE_HPP
#define KASTEN_STRUCTUREITEMDELEGATE_HPP

// KF
#include <KWidgetItemDelegate>

class QCheckBox;
class QPushButton;

namespace Kasten {

class StructureItemDelegate : public KWidgetItemDelegate
{
    Q_OBJECT

public:
    StructureItemDelegate(QAbstractItemView* itemView, QObject* parent);
    ~StructureItemDelegate() override;

public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    [[nodiscard]]
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

Q_SIGNALS:
    void uninstallStructureRequested(const QModelIndex& index);

protected:
    [[nodiscard]]
    QList<QWidget*> createItemWidgets(const QModelIndex& index) const override;
    void updateItemWidgets(const QList<QWidget*>& widgets, const QStyleOptionViewItem& option, const QPersistentModelIndex& index) const override;

private Q_SLOTS:
    void handleUninstallClicked();
    void handleAboutClicked();
    void handleCheckClicked(bool checked);

private:
    enum LayoutMode {
        LayoutForActualSize, ///< layout with rects for where objects are exeactly placed
        LayoutForSizeHint, ///< layout with sizes set to hinted ones, positions are ignored
    };
    void layoutItem(const QStyleOptionViewItem& option,
                    const QModelIndex& index,
                    QRect* checkBoxRect, QRect* iconRect, QRect* textRect,
                    QRect* aboutButtonRect, QRect* uninstallButtonRect,
                    int* buttonHSpacing,
                    int* focusFrameHMarginToUpdate,
                    LayoutMode layoutMode) const;

private:
    std::unique_ptr<QCheckBox> m_sampleCheckBox;
    std::unique_ptr<QPushButton> m_sampleButton;
};

}

#endif

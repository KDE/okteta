/*
    SPDX-FileCopyrightText: 2022 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureitemdelegate.hpp"

// tool
#include "structuresselectionmodel.hpp"
#include "aboutstructuredialog.hpp"
// KF
#include <KCategorizedView>
#include <KLocalizedString>
// Qt
#include <QApplication>
#include <QStyleOptionViewItem>
#include <QCheckBox>
#include <QPushButton>
#include <QStyle>
#include <QPainter>
#include <QFont>
// Std
#include <algorithm>

namespace {
static QFont nameFont(const QFont& font)
{
    QFont nameFont(font);
    nameFont.setBold(true);

    return nameFont;
}
}

namespace Kasten {

StructureItemDelegate::StructureItemDelegate(QAbstractItemView* itemView, QObject* parent)
    : KWidgetItemDelegate(itemView, parent)
    , m_sampleCheckBox(new QCheckBox)
    , m_sampleButton(new QPushButton)
{
    m_sampleButton->setIcon(QIcon::fromTheme(QStringLiteral("configure"))); // only for getting size matters
}

StructureItemDelegate::~StructureItemDelegate() = default;

void StructureItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const
{
    if (!index.isValid()) {
        return;
    }

    QRect iconRect;
    QRect textRect;
    layoutItem(option, index, nullptr, &iconRect, &textRect, nullptr, nullptr, nullptr, nullptr, LayoutForActualSize);

    painter->save();

    // draw background
    const QWidget* const widget = option.widget;
    QStyle* const style = widget ? widget->style() : QApplication::style();
    style->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, painter, widget);

    // draw icon
    const QIcon icon = QIcon::fromTheme(index.model()->data(index, Qt::DecorationRole).toString());
    icon.paint(painter, iconRect);

    // draw text
    if (option.state & QStyle::State_Selected) {
        painter->setPen(option.palette.highlightedText().color());
    }

    // ... name
    painter->save();
    const QFont nameFont = ::nameFont(option.font);
    QFontMetrics nameFontMetrics(nameFont);
    painter->setFont(nameFont);
    painter->drawText(textRect,
                      Qt::AlignLeft | Qt::AlignTop,
                      nameFontMetrics.elidedText(index.model()->data(index, Qt::DisplayRole).toString(), Qt::ElideRight, textRect.width()));
    painter->restore();

    // ... comment
    painter->drawText(textRect,
                      Qt::AlignLeft | Qt::AlignBottom,
                      option.fontMetrics.elidedText(index.model()->data(index, StructuresSelectionModel::CommentRole).toString(), Qt::ElideRight, textRect.width()));

    painter->restore();
}

QSize StructureItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QRect checkBoxRect;
    QRect iconRect;
    QRect textRect;
    QRect aboutButtonRect;
    QRect uninstallButtonRect;
    int focusFrameHMargin;
    int buttonHSpacing;
    layoutItem(option, index, &checkBoxRect, &iconRect, &textRect, &aboutButtonRect, &uninstallButtonRect, &buttonHSpacing, &focusFrameHMargin, LayoutForSizeHint);

    const int width = checkBoxRect.width() + iconRect.width() + textRect.width() + aboutButtonRect.width() + uninstallButtonRect.width() + buttonHSpacing + 8 * focusFrameHMargin;
    const int height = std::max(checkBoxRect.height(), std::max(iconRect.height(), std::max(textRect.height(), aboutButtonRect.height()))) + 2 * focusFrameHMargin;
    return {width, height};
}

QList<QWidget*> StructureItemDelegate::createItemWidgets(const QModelIndex& index) const
{
    auto* const enabledCheckBox = new QCheckBox;
    connect(enabledCheckBox, &QAbstractButton::clicked,
            this, &StructureItemDelegate::handleCheckClicked);

    auto* const aboutButton = new QPushButton;
    aboutButton->setIcon(QIcon::fromTheme(QStringLiteral("dialog-information")));
    aboutButton->setToolTip(i18nc("@info:tooltip", "About"));
    connect(aboutButton, &QAbstractButton::clicked,
            this, &StructureItemDelegate::handleAboutClicked);

    auto* const uninstallButton = new QPushButton;
    uninstallButton->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete")));
    const bool isUninstallable = index.data(StructuresSelectionModel::UninstallableRole).toBool();
    uninstallButton->setToolTip(isUninstallable ? i18nc("@info:tooltip", "Uninstall") : i18nc("@info:tooltip", "Can not be uninstalled."));
    uninstallButton->setEnabled(isUninstallable);
    connect(uninstallButton, &QAbstractButton::clicked,
            this, &StructureItemDelegate::handleUninstallClicked);

    const QList<QEvent::Type> blockedEventTypes {
        QEvent::MouseButtonPress,
        QEvent::MouseButtonRelease,
        QEvent::MouseButtonDblClick,
        QEvent::KeyPress,
        QEvent::KeyRelease,
    };
    setBlockedEventTypes(enabledCheckBox, blockedEventTypes);
    setBlockedEventTypes(aboutButton, blockedEventTypes);
    setBlockedEventTypes(uninstallButton, blockedEventTypes);

    const QList<QWidget*> widgetList {
        enabledCheckBox,
        aboutButton,
        uninstallButton,
    };

    return widgetList;
}

void StructureItemDelegate::updateItemWidgets(const QList<QWidget*>& widgets,
                                              const QStyleOptionViewItem &option,
                                              const QPersistentModelIndex& index) const
{
    QRect checkBoxRect;
    QRect aboutButtonRect;
    QRect uninstallButtonRect;
    layoutItem(option, index, &checkBoxRect, nullptr, nullptr, &aboutButtonRect, &uninstallButtonRect, nullptr, nullptr, LayoutForActualSize);

    // check box
    auto* const checkBox = static_cast<QCheckBox*>(widgets[0]);
    checkBox->resize(checkBoxRect.size());
    checkBox->move(checkBoxRect.topLeft());

    // about button
    auto* const aboutButton = static_cast<QPushButton *>(widgets[1]);
    aboutButton->resize(aboutButtonRect.size());
    aboutButton->move(aboutButtonRect.topLeft());

    // uninstall button
    auto* const uninstallButton = static_cast<QPushButton *>(widgets[2]);
    uninstallButton->resize(uninstallButtonRect.size());
    uninstallButton->move(uninstallButtonRect.topLeft());

    if (!index.isValid()) {
        checkBox->setVisible(false);
        aboutButton->setVisible(false);
        uninstallButton->setVisible(false);
    } else {
        checkBox->setChecked(index.data(Qt::CheckStateRole).toBool());
    }
}

void StructureItemDelegate::layoutItem(const QStyleOptionViewItem& option,
                                       const QModelIndex& index,
                                       QRect* checkBoxRectToUpdate,
                                       QRect* iconRectToUpdate,
                                       QRect* textRectToUpdate,
                                       QRect* aboutButtonRectToUpdate,
                                       QRect* uninstallButtonRectToUpdate,
                                       int* buttonHSpacingToUpdate,
                                       int* focusFrameHMarginToUpdate,
                                       LayoutMode layoutMode) const
{
    const QWidget* const widget = option.widget;
    QStyle* const style = widget ? widget->style() : QApplication::style();
    // "+ 1" as used in QItemDelegate
    const int buttonHSpacing = style->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, &option, widget);
    if (buttonHSpacingToUpdate) {
        *buttonHSpacingToUpdate = buttonHSpacing;
    }
    const int focusFrameHMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, &option, widget) + 1;
    if (focusFrameHMarginToUpdate) {
        *focusFrameHMarginToUpdate = focusFrameHMargin;
    }

    const QSize checkBoxSize = m_sampleCheckBox->sizeHint();
    const QSize buttonSize = m_sampleButton->sizeHint();
    const int marginedTop = option.rect.top() + focusFrameHMargin;
    const int marginedHeight = option.rect.height() - 2 * focusFrameHMargin;
    // KWidgetItemDelegate::updateItemWidgets(...) docs say:
    // "All the positioning and sizing should be done in item coordinates."
    // So translating the positions when updating the widget rects
    const QPoint widgetTranslation(-option.rect.left(), -option.rect.top());

    const int checkBoxXOffset = option.rect.left() + ((option.direction == Qt::LeftToRight) ?
        focusFrameHMargin :
        (option.rect.width() - checkBoxSize.width() - focusFrameHMargin));
    const int checkBoxYOffset = marginedTop  + (marginedHeight - checkBoxSize.height()) / 2;

    const QRect checkBoxRect(QPoint(checkBoxXOffset, checkBoxYOffset), checkBoxSize);
    if (checkBoxRectToUpdate) {
        *checkBoxRectToUpdate = checkBoxRect.translated(widgetTranslation);
    }

    const int iconSize = style->pixelMetric(QStyle::PM_IconViewIconSize);
    const int iconXOffset = (option.direction == Qt::LeftToRight) ?
        (checkBoxRect.right() + 1 + 2 * focusFrameHMargin) :
        (checkBoxRect.left() - iconSize - 2 * focusFrameHMargin);
    const int iconYOffset = marginedTop + (marginedHeight - iconSize) / 2;

    const QRect iconRect(iconXOffset, iconYOffset, iconSize, iconSize);
    if (iconRectToUpdate) {
        *iconRectToUpdate = iconRect;
    }

    const int buttonYOffset = marginedTop + (marginedHeight - buttonSize.height()) / 2;

    const int uninstallButtonXOffset = option.rect.left() + ((option.direction == Qt::LeftToRight) ?
        (option.rect.width() - buttonSize.width() - focusFrameHMargin) :
        focusFrameHMargin);

    const QRect uninstallButtonRect(QPoint(uninstallButtonXOffset, buttonYOffset), buttonSize);
    if (uninstallButtonRectToUpdate) {
        *uninstallButtonRectToUpdate = uninstallButtonRect.translated(widgetTranslation);
    }

    const int aboutButtonXOffset = (option.direction == Qt::LeftToRight) ?
        (uninstallButtonRect.left() - buttonSize.width() - buttonHSpacing) :
        (uninstallButtonRect.right() + buttonHSpacing);

    const QRect aboutButtonRect(QPoint(aboutButtonXOffset, buttonYOffset), buttonSize);
    if (aboutButtonRectToUpdate) {
        *aboutButtonRectToUpdate = aboutButtonRect.translated(widgetTranslation);
    }

    if (textRectToUpdate) {
        if (layoutMode == LayoutForSizeHint) {
            const QFont nameFont = ::nameFont(option.font);
            const QFontMetrics nameFontMetrics(nameFont);
            const QString name = index.model()->data(index, Qt::DisplayRole).toString();
            const QString comment = index.model()->data(index, StructuresSelectionModel::CommentRole).toString();
            const int contentWidthHint = std::max(nameFontMetrics.boundingRect(name).width(), option.fontMetrics.boundingRect(comment).width());
            const int contentHeightHint = nameFontMetrics.height() + option.fontMetrics.height();
            *textRectToUpdate = QRect(0, 0, contentWidthHint, contentHeightHint);
        } else {
            const int contentHeight = option.rect.height() - 2 * focusFrameHMargin;
            const int contentWidth = ((option.direction == Qt::LeftToRight) ?
                (aboutButtonRect.left() - iconRect.right()) :
                (iconRect.left() - aboutButtonRect.right()))
                - 1 - 4 * focusFrameHMargin;
            const int contentXOffset = ((option.direction == Qt::LeftToRight) ?
                iconRect.right() : aboutButtonRect.right()) + 1 + 2 * focusFrameHMargin;
            const int contentYOffset = marginedTop;
            *textRectToUpdate = QRect(contentXOffset, contentYOffset, contentWidth, contentHeight);
        }
    }
}

void StructureItemDelegate::handleAboutClicked()
{
    const QModelIndex index = focusedIndex();
    if (!index.isValid()) {
        return;
    }

    const auto metaData = index.data(StructuresSelectionModel::MetaDataRole).value<StructureMetaData>();

    auto* const aboutDialog = new AboutStructureDialog(metaData, itemView());
    aboutDialog->show();
}

void StructureItemDelegate::handleUninstallClicked()
{
    const QModelIndex index = focusedIndex();
    if (!index.isValid()) {
        return;
    }

    Q_EMIT uninstallStructureRequested(index);
}

void StructureItemDelegate::handleCheckClicked(bool checked)
{
    const QModelIndex index = focusedIndex();

    if (!index.isValid()) {
        return;
    }
    auto* const model = const_cast<QAbstractItemModel *>(index.model());

    model->setData(index, checked, Qt::CheckStateRole);
}

}

#include "moc_structureitemdelegate.cpp"

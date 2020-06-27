/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "gotooffsetview.hpp"

// tool
#include "gotooffsettool.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>
#include <Kasten/Okteta/AddressValidator>
// KF
#include <QPushButton>
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QLabel>
#include <QLayout>

namespace Kasten {

GotoOffsetView::GotoOffsetView(GotoOffsetTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // offset
    auto* offsetLayout = new QHBoxLayout();
    offsetLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* label = new QLabel(i18nc("@label:listbox", "O&ffset:"), this);
    mAddressEdit = new Okteta::AddressComboBox(this);
    connect(mAddressEdit, &Okteta::AddressComboBox::addressChanged,
            mTool, &GotoOffsetTool::setTargetOffset);
    connect(mAddressEdit, &Okteta::AddressComboBox::formatChanged,
            this, &GotoOffsetView::onFormatChanged);
    connect(mAddressEdit, &Okteta::AddressComboBox::addressTypeChanged,
            this, &GotoOffsetView::onAddressTypeChanged);
    label->setBuddy(mAddressEdit);
    const QString inputWhatsThis =
        i18nc("@info:whatsthis", "Enter an offset to go to, or select a previous offset from the list.");
    label->setWhatsThis(inputWhatsThis);
    mAddressEdit->setWhatsThis(inputWhatsThis);

    offsetLayout->addWidget(label);
    offsetLayout->addWidget(mAddressEdit, 1);

    baseLayout->addLayout(offsetLayout, 1);
    baseLayout->setAlignment(offsetLayout, Qt::AlignTop);

    setFocusProxy(mAddressEdit);   // TODO: see how KDialog does it, e.g. see if there is already a focuswidget as child

    // options
    auto* optionsLayout = new QVBoxLayout();
    optionsLayout->setContentsMargins(0, 0, 0, 0);

    mAtCursorCheckBox = new QCheckBox(i18nc("@option:check", "From c&ursor"), this);
    mAtCursorCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Go relative from the current cursor location and not absolute."));
    connect(mAtCursorCheckBox, &QCheckBox::toggled,
            mTool, &GotoOffsetTool::setIsRelative);
    mExtendSelectionCheckBox = new QCheckBox(i18nc("@option:check", "&Extend selection"), this);
    mExtendSelectionCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Extend the selection by the cursor move."));
    connect(mExtendSelectionCheckBox, &QCheckBox::toggled,
            mTool, &GotoOffsetTool::setIsSelectionToExtent);
    mBackwardsCheckBox = new QCheckBox(i18nc("@option:check", "&Backwards"), this);
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Go backwards from the end or the current cursor location."));
    connect(mBackwardsCheckBox, &QCheckBox::toggled, mTool, &GotoOffsetTool::setIsBackwards);

    auto* upperOptionsLayout = new QHBoxLayout();
    upperOptionsLayout->setContentsMargins(0, 0, 0, 0);
    upperOptionsLayout->addWidget(mAtCursorCheckBox);
    upperOptionsLayout->addWidget(mBackwardsCheckBox);

    optionsLayout->addLayout(upperOptionsLayout);
    optionsLayout->addWidget(mExtendSelectionCheckBox);

    baseLayout->addLayout(optionsLayout);

    // Goto button
    const KGuiItem gotoGuiItem =
        KGuiItem(i18nc("@action:button", "&Go"),
                 QStringLiteral("go-jump"),
                 i18nc("@info:tooltip",
                       "Go to the Offset"),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Go</interface> "
                        "button, the cursor will be moved in the document to or, "
                        "on your option, by the offset you entered above."));
    mGotoButton = new QPushButton(this);
    KGuiItem::assign(mGotoButton, gotoGuiItem);
    connect(mGotoButton, &QPushButton::clicked, this, &GotoOffsetView::onGotoButtonClicked);
    addButton(mGotoButton, AbstractToolWidget::Default);
    baseLayout->addWidget(mGotoButton);
    baseLayout->setAlignment(mGotoButton, Qt::AlignTop);

    setTabOrder(mAddressEdit, mAtCursorCheckBox);
    setTabOrder(mAtCursorCheckBox, mBackwardsCheckBox);
    setTabOrder(mBackwardsCheckBox, mExtendSelectionCheckBox);
    setTabOrder(mExtendSelectionCheckBox, mGotoButton);

    connect(mTool, &GotoOffsetTool::isApplyableChanged,
            this, &GotoOffsetView::onApplyableChanged);

    onApplyableChanged(mTool->isApplyable());
}

GotoOffsetView::~GotoOffsetView() = default;

void GotoOffsetView::onApplyableChanged(bool isApplyable)
{
    // TODO: set error tooltip, like offset out of range or no document
    // TODO: set color flag to offset input
    mGotoButton->setEnabled(isApplyable);
}

void GotoOffsetView::onGotoButtonClicked()
{
    // TODO: collect recently used offset in tool instead?
    mAddressEdit->rememberCurrentAddress();

    mTool->gotoOffset();
//     emit toolUsed();
}

void GotoOffsetView::onAddressTypeChanged(int addressType)
{
    const bool isNotExpression = (mAddressEdit->format() != 2);
    if (isNotExpression
        || addressType == Okteta::AddressValidator::InvalidAddressType) {
        return;
    }

    bool fromCursor = false;
    bool backwards = false;

    if (addressType == Okteta::AddressValidator::AbsoluteAddress) {
        fromCursor = false;
        backwards = false; // TODO: there is no way yet for: absolute from end
    } else if (addressType == Okteta::AddressValidator::RelativeForwards) {
        fromCursor = true;
        backwards = false;
    } else if (addressType == Okteta::AddressValidator::RelativeBackwards) {
        fromCursor = true;
        backwards = true;
    }

    mAtCursorCheckBox->setChecked(fromCursor);
    mTool->setIsRelative(fromCursor);
    mBackwardsCheckBox->setChecked(backwards);
    mTool->setIsBackwards(backwards);
}

void GotoOffsetView::onFormatChanged(int formatIndex)
{
    // TODO: make sure Expr is always at index 2
    const bool isNotExpression = (formatIndex != 2);

    mAtCursorCheckBox->setEnabled(isNotExpression);
    mBackwardsCheckBox->setEnabled(isNotExpression);
}

}

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangeview.hpp"

// tool
#include "selectrangetool.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>
// KF
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>

namespace Kasten {

SelectRangeView::SelectRangeView(SelectRangeTool* tool, QWidget* parent)
    : AbstractToolWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QHBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // offsets
    auto* offsetLayout = new QVBoxLayout();
    offsetLayout->setContentsMargins(0, 0, 0, 0);

    // start offset
    auto* startOffsetLayout = new QHBoxLayout();
    startOffsetLayout->setContentsMargins(0, 0, 0, 0);

    QLabel* label = new QLabel(i18nc("@label:listbox", "Start offset:"), this);
    mStartEdit = new Okteta::AddressComboBox(this);
    connect(mStartEdit, &Okteta::AddressComboBox::addressChanged,
            mTool, &SelectRangeTool::setTargetStart);
    label->setBuddy(mStartEdit);
    const QString startInputWhatsThis =
        i18nc("@info:whatsthis", "Enter an offset to go to, or select a previous offset from the list.");
    label->setWhatsThis(startInputWhatsThis);
    mStartEdit->setWhatsThis(startInputWhatsThis);

    startOffsetLayout->addWidget(label);
    startOffsetLayout->addWidget(mStartEdit);
    setFocusProxy(mStartEdit);   // TODO: see how KDialog does it, e.g. see if there is already a focuswidget as child

    offsetLayout->addLayout(startOffsetLayout);

    // end offset
    auto* endOffsetLayout = new QHBoxLayout();
    endOffsetLayout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(i18nc("@label:listbox", "End offset:"), this);
    mEndEdit = new Okteta::AddressComboBox(this);
    connect(mEndEdit, &Okteta::AddressComboBox::addressChanged,
            mTool, &SelectRangeTool::setTargetEnd);
    label->setBuddy(mEndEdit);
    const QString endInputWhatsThis =
        i18nc("@info:whatsthis", "Enter an offset to go to, or select a previous offset from the list.");
    label->setWhatsThis(endInputWhatsThis);
    mEndEdit->setWhatsThis(endInputWhatsThis);

    endOffsetLayout->addWidget(label);
    endOffsetLayout->addWidget(mEndEdit);

    offsetLayout->addLayout(endOffsetLayout);
    baseLayout->addLayout(offsetLayout);

    // options
    auto* optionsLayout = new QVBoxLayout();
    optionsLayout->setContentsMargins(0, 0, 0, 0);

    mRelativeCheckBox = new QCheckBox(i18nc("@option:check", "End relative"), this);
    mRelativeCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Extend the selection by the cursor move."));
    connect(mRelativeCheckBox, &QCheckBox::toggled,
            mTool, &SelectRangeTool::setIsEndRelative);
    mRelativeCheckBox->setChecked(mTool->isEndRelative());
    mBackwardsCheckBox = new QCheckBox(i18nc("@option:check", "&Backwards"), this);
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Go backwards from the end or the current cursor location."));
    connect(mBackwardsCheckBox, &QCheckBox::toggled,
            mTool, &SelectRangeTool::setIsEndBackwards);
    mBackwardsCheckBox->setChecked(mTool->isEndBackwards());

    connect(mRelativeCheckBox, &QCheckBox::toggled, mBackwardsCheckBox, &QCheckBox::setEnabled);
    mBackwardsCheckBox->setEnabled(mRelativeCheckBox->isChecked());

    optionsLayout->addWidget(mRelativeCheckBox);
    optionsLayout->addWidget(mBackwardsCheckBox);

    baseLayout->addLayout(optionsLayout);

    // Select button
    const KGuiItem selectGuiItem =
        KGuiItem(i18nc("@action:button",
                       "&Select"),
                 QString(),
                 i18nc("@info:tooltip",
                       "Select the range."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Select</interface> "
                        "button, the cursor will be moved in the document to or, "
                        "on your option, by the offset you entered above."));
    mSelectButton = new QPushButton(this);
    KGuiItem::assign(mSelectButton, selectGuiItem);
    connect(mSelectButton, &QPushButton::clicked, this, &SelectRangeView::onSelectButtonClicked);
    addButton(mSelectButton, AbstractToolWidget::Default);
    baseLayout->addWidget(mSelectButton);
    baseLayout->setAlignment(mSelectButton, Qt::AlignTop);

    baseLayout->addStretch();

    setTabOrder(mStartEdit, mEndEdit);
    setTabOrder(mEndEdit, mRelativeCheckBox);
    setTabOrder(mRelativeCheckBox, mBackwardsCheckBox);
    setTabOrder(mBackwardsCheckBox, mSelectButton);

    connect(mTool, &SelectRangeTool::isApplyableChanged, this, &SelectRangeView::onApplyableChanged);

    onApplyableChanged(mTool->isApplyable());
}

SelectRangeView::~SelectRangeView() = default;

void SelectRangeView::onApplyableChanged(bool isApplyable)
{
    // TODO: set error tooltip, like offset out of range or no document
    // TODO: set color flag to offset input
    mSelectButton->setEnabled(isApplyable);
}

void SelectRangeView::onSelectButtonClicked()
{
    // TODO: collect recently used offsets in tool instead?
    mStartEdit->rememberCurrentAddress();
    mEndEdit->rememberCurrentAddress();

    mTool->select();
//     emit toolUsed();
}

}

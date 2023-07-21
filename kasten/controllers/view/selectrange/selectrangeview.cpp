/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "selectrangeview.hpp"

// tool
#include "selectrangetool.hpp"
// libconfigentries
#include <addresscomboboxcodingconfigentry.hpp>
#include <directionconfigentry.hpp>
// Okteta Kasten gui
#include <Kasten/Okteta/AddressComboBox>
// KF
#include <KGuiItem>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>


template <>
inline Kasten::SelectRangeView::SelectDirection KConfigGroup::readEntry(const char *key, const Kasten::SelectRangeView::SelectDirection &defaultValue) const
{
    return static_cast<Kasten::SelectRangeView::SelectDirection>(KConfigGroup::readEntry(key, static_cast<Kasten::Direction>(defaultValue)));
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::SelectRangeView::SelectDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    writeEntry(key, static_cast<Kasten::Direction>(value), flags);
}

namespace Kasten {

// C++11 needs a definition for static constexpr members
constexpr char SelectRangeView::ConfigGroupId[];

constexpr char SelectRangeView::StartOffsetCodingConfigKey[];
constexpr char SelectRangeView::EndOffsetCodingConfigKey[];
constexpr char SelectRangeView::RelativeToEndConfigKey[];
constexpr char SelectRangeView::DirectionConfigKey[];

constexpr Okteta::AddressComboBox::Coding SelectRangeView::DefaultStartOffsetCoding;
constexpr Okteta::AddressComboBox::Coding SelectRangeView::DefaultEndOffsetCoding;
constexpr bool SelectRangeView::DefaultRelativeToEnd;
constexpr SelectRangeView::SelectDirection SelectRangeView::DefaultDirection;


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

    mBackwardsCheckBox = new QCheckBox(i18nc("@option:check", "&Backwards"), this);
    mBackwardsCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Go backwards from the end or the current cursor location."));
    connect(mBackwardsCheckBox, &QCheckBox::toggled,
            mTool, &SelectRangeTool::setIsEndBackwards);
    mBackwardsCheckBox->setChecked(mTool->isEndBackwards());
    mRelativeCheckBox = new QCheckBox(i18nc("@option:check", "End relative"), this);
    mRelativeCheckBox->setWhatsThis(
        i18nc("@info:whatsthis", "Extend the selection by the cursor move."));
    connect(mRelativeCheckBox, &QCheckBox::toggled,
            mTool, &SelectRangeTool::setIsEndRelative);
    mRelativeCheckBox->setChecked(mTool->isEndRelative());

    connect(mRelativeCheckBox, &QCheckBox::toggled, mBackwardsCheckBox, &QCheckBox::setEnabled);
    mBackwardsCheckBox->setEnabled(mRelativeCheckBox->isChecked());

    optionsLayout->addWidget(mBackwardsCheckBox);
    optionsLayout->addWidget(mRelativeCheckBox);

    baseLayout->addLayout(optionsLayout);

    // Select button
    const KGuiItem selectGuiItem =
        KGuiItem(i18nc("@action:button",
                       "&Select"),
                 QStringLiteral("select-rectangular"),
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

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    const Okteta::AddressComboBox::Coding startOffsetCoding = configGroup.readEntry(StartOffsetCodingConfigKey, DefaultStartOffsetCoding);
    mStartEdit->setFormat(startOffsetCoding);

    const Okteta::AddressComboBox::Coding endOffsetCoding = configGroup.readEntry(EndOffsetCodingConfigKey, DefaultEndOffsetCoding);
    mEndEdit->setFormat(endOffsetCoding);

    const SelectDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    mBackwardsCheckBox->setChecked(direction == SelectBackward);

    const bool relativeToEnd = configGroup.readEntry(RelativeToEndConfigKey, DefaultRelativeToEnd);
    mRelativeCheckBox->setChecked(relativeToEnd);

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

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(StartOffsetCodingConfigKey,
                           static_cast<Okteta::AddressComboBox::Coding>(mStartEdit->format()));
    configGroup.writeEntry(EndOffsetCodingConfigKey,
                           static_cast<Okteta::AddressComboBox::Coding>(mEndEdit->format()));
    configGroup.writeEntry(DirectionConfigKey,
                           mBackwardsCheckBox->isChecked() ? SelectBackward : SelectForward);
    configGroup.writeEntry(RelativeToEndConfigKey, mRelativeCheckBox->isChecked());

    mTool->select();
//     emit toolUsed();
}

}

#include "moc_selectrangeview.cpp"

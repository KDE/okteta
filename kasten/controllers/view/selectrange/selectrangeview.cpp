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
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QLayout>
// Std
#include <algorithm>
#include <array>
#include <iterator>

// TODO: move to helper interface lib?
// Matching Okteta::AddressComboBox::Coding
static constexpr int codingCount = 3;
static const std::array<QString, codingCount> codingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Expression"),
};


template <>
inline Okteta::AddressComboBox::Coding
KConfigGroup::readEntry(const char *key,
                        const Okteta::AddressComboBox::Coding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(codingConfigValueList.cbegin(), codingConfigValueList.cend(), entry);
    if (it == codingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(codingConfigValueList.cbegin(), it);
    return static_cast<Okteta::AddressComboBox::Coding>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Okteta::AddressComboBox::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == Okteta::AddressComboBox::InvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = codingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}

namespace Kasten
{
enum SelectDirection
{
    SelectForward = 0,
    SelectBackward = 1
};
}

template <>
inline Kasten::SelectDirection KConfigGroup::readEntry(const char *key, const Kasten::SelectDirection &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Kasten::SelectDirection direction =
        (entry == QLatin1String("Forward")) ?  Kasten::SelectForward :
        (entry == QLatin1String("Backward")) ? Kasten::SelectBackward :
        /* else */                             defaultValue;
    return direction;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::SelectDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Kasten::SelectForward) ? QLatin1String("Forward") : QLatin1String("Backward");
    writeEntry(key, valueString, flags);
}

static constexpr Okteta::AddressComboBox::Coding DefaultStartOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;
static constexpr Okteta::AddressComboBox::Coding DefaultEndOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;
static constexpr bool DefaultRelativeToEnd = false;
static constexpr Kasten::SelectDirection DefaultDirection = Kasten::SelectForward;

static constexpr char SelectRangeConfigGroupId[] = "SelectRangeTool";

static constexpr char StartOffsetCodingConfigKey[] = "StartOffsetCoding";
static constexpr char EndOffsetCodingConfigKey[] = "EndOffsetCoding";
static constexpr char RelativeToEndConfigKey[] = "RelativeToEnd";
static constexpr char DirectionConfigKey[] = "Direction";

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

    const KConfigGroup configGroup(KSharedConfig::openConfig(), SelectRangeConfigGroupId);

    const Okteta::AddressComboBox::Coding startOffsetCoding = configGroup.readEntry(StartOffsetCodingConfigKey, DefaultStartOffsetCoding);
    mStartEdit->setFormat(startOffsetCoding);

    const Okteta::AddressComboBox::Coding endOffsetCoding = configGroup.readEntry(EndOffsetCodingConfigKey, DefaultEndOffsetCoding);
    mEndEdit->setFormat(endOffsetCoding);

    const Kasten::SelectDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
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

    KConfigGroup configGroup(KSharedConfig::openConfig(), SelectRangeConfigGroupId);
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

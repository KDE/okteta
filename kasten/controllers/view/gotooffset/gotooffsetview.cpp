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
enum GotoDirection
{
    GotoForward = 0,
    GotoBackward = 1
};
}

template <>
inline Kasten::GotoDirection KConfigGroup::readEntry(const char *key, const Kasten::GotoDirection &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Kasten::GotoDirection direction =
        (entry == QLatin1String("Forward")) ?  Kasten::GotoForward :
        (entry == QLatin1String("Backward")) ? Kasten::GotoBackward :
        /* else */                             defaultValue;
    return direction;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::GotoDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Kasten::GotoForward) ? QLatin1String("Forward") : QLatin1String("Backward");
    writeEntry(key, valueString, flags);
}

static constexpr bool DefaultFromCursor = false;
static constexpr bool DefaultExtendSelection = false;
static constexpr Kasten::GotoDirection DefaultDirection = Kasten::GotoForward;
static constexpr Okteta::AddressComboBox::Coding DefaultOffsetCoding = Okteta::AddressComboBox::HexadecimalCoding;

static constexpr char GotoOffsetConfigGroupId[] = "GotoOffsetTool";

static constexpr char OffsetCodingConfigKey[] = "OffsetCoding";
static constexpr char FromCursorConfigKey[] = "FromCursor";
static constexpr char ExtendSelectionConfigKey[] = "ExtendSelection";
static constexpr char DirectionConfigKey[] = "Direction";

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

    auto* label = new QLabel(i18nc("@label:listbox", "O&ffset:"), this);
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

    const KConfigGroup configGroup(KSharedConfig::openConfig(), GotoOffsetConfigGroupId);

    const bool fromCursor = configGroup.readEntry(FromCursorConfigKey, DefaultFromCursor);
    mAtCursorCheckBox->setChecked(fromCursor);

    const Kasten::GotoDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    mBackwardsCheckBox->setChecked(direction == GotoBackward);

    const bool extendSelection = configGroup.readEntry(ExtendSelectionConfigKey, DefaultExtendSelection);
    mExtendSelectionCheckBox->setChecked(extendSelection);

    const Okteta::AddressComboBox::Coding offsetCoding = configGroup.readEntry(OffsetCodingConfigKey, DefaultOffsetCoding);
    mAddressEdit->setFormat(offsetCoding);

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

    KConfigGroup configGroup(KSharedConfig::openConfig(), GotoOffsetConfigGroupId);
    configGroup.writeEntry(OffsetCodingConfigKey, static_cast<Okteta::AddressComboBox::Coding>(mAddressEdit->format()));
    configGroup.writeEntry(DirectionConfigKey,
                           mBackwardsCheckBox->isChecked() ? GotoBackward : GotoForward);
    configGroup.writeEntry(ExtendSelectionConfigKey, mExtendSelectionCheckBox->isChecked());
    configGroup.writeEntry(FromCursorConfigKey, mAtCursorCheckBox->isChecked());

    mTool->gotoOffset();
//     Q_EMIT toolUsed();
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

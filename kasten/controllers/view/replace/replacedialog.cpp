/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacedialog.hpp"

// controller
#include "replacetool.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>
// Std
#include <algorithm>
#include <array>
#include <iterator>

// TODO: move to helper interface lib?
// Matching Okteta::ByteArrayComboBox::Coding
static constexpr int codingCount = 6;
static const std::array<QString, codingCount> codingConfigValueList = {
    QStringLiteral("Hexadecimal"),
    QStringLiteral("Decimal"),
    QStringLiteral("Octal"),
    QStringLiteral("Binary"),
    QStringLiteral("Char"),
    QStringLiteral("UTF-8"),
};


template <>
inline Okteta::ByteArrayComboBox::Coding
KConfigGroup::readEntry(const char *key,
                        const Okteta::ByteArrayComboBox::Coding &defaultValue) const
{
    const QString entry = readEntry(key, QString());

    auto it = std::find(codingConfigValueList.cbegin(), codingConfigValueList.cend(), entry);
    if (it == codingConfigValueList.cend()) {
        return defaultValue;
    }

    const int listIndex = std::distance(codingConfigValueList.cbegin(), it);
    return static_cast<Okteta::ByteArrayComboBox::Coding>(listIndex);
}

template <>
inline void KConfigGroup::writeEntry(const char *key,
                                     const Okteta::ByteArrayComboBox::Coding &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    QString configValue;
    if (value == Okteta::ByteArrayComboBox::InvalidCoding) {
        configValue = QStringLiteral("Invalid");
    } else {
        const int listIndex = static_cast<int>(value);
        configValue = codingConfigValueList[listIndex];
    }
    writeEntry(key, configValue, flags);
}

template <>
inline Kasten::FindDirection KConfigGroup::readEntry(const char *key, const Kasten::FindDirection &defaultValue) const
{
    const QString entry = readEntry(key, QString());
    const Kasten::FindDirection direction =
        (entry == QLatin1String("Forward")) ?  Kasten::FindForward :
        (entry == QLatin1String("Backward")) ? Kasten::FindBackward :
        /* else */                             defaultValue;
    return direction;
}

template <>
inline void KConfigGroup::writeEntry(const char *key, const Kasten::FindDirection &value,
                                     KConfigBase::WriteConfigFlags flags)
{
    const QString valueString =
        (value == Kasten::FindForward) ? QLatin1String("Forward") : QLatin1String("Backward");
    writeEntry(key, valueString, flags);
}

static constexpr bool DefaultFromCursor = false;
static constexpr Kasten::FindDirection DefaultDirection = Kasten::FindForward;
static constexpr Okteta::ByteArrayComboBox::Coding DefaultReplaceDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;
static constexpr Okteta::ByteArrayComboBox::Coding DefaultSearchDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;

static constexpr char ReplaceConfigGroupId[] = "ReplaceTool";

static constexpr char FromCursorConfigKey[] = "FromCursor";
static constexpr char DirectionConfigKey[] = "Direction";
static constexpr char ReplaceDataCodingConfigKey[] = "ReplaceDataCoding";
static constexpr char SearchDataCodingConfigKey[] = "SearchDataCoding";

namespace Kasten {

ReplaceDialog::ReplaceDialog(ReplaceTool* tool, QWidget* parent)
    : AbstractFindDialog(parent)
    , mTool(tool)
{
    setWindowTitle(i18nc("@title:window", "Replace Bytes"));

    setFindButton(i18nc("@action;button", "&Replace"),
                  QStringLiteral("edit-find-replace"),
                  i18nc("@info:tooltip", "Start replace"),
                  xi18nc("@info:whatsthis",
                         "If you press the <interface>Replace</interface> button, "
                         "the bytes you entered above are searched for within "
                         "the byte array and any occurrence is replaced with "
                         "the replacement bytes."));

    setupFindBox();

    // replace term
    QGroupBox* ReplaceBox = new QGroupBox(i18nc("@title:group", "Replace With"));

    auto* ReplaceBoxLayout = new QVBoxLayout;

    ReplaceDataEdit = new Okteta::ByteArrayComboBox(ReplaceBox);
    const QString toolTip =
        i18nc("@info:tooltip",
              "Enter the bytes to replace with, or select bytes previously replaced with from the list.");
    ReplaceDataEdit->setToolTip(toolTip);

    ReplaceBoxLayout->addWidget(ReplaceDataEdit);

    ReplaceBox->setLayout(ReplaceBoxLayout);
    setupOperationBox(ReplaceBox);

    //
    PromptCheckBox = new QCheckBox(i18nc("@option:check", "&Prompt on replace"));
    PromptCheckBox->setWhatsThis(i18nc("@info:whatsthis", "Ask before replacing each match found."));

    setupCheckBoxes(PromptCheckBox);

    setFindButtonEnabled(false);
    setModal(true);

    //
    setCaseSensitivity(mTool->caseSensitivity());
    PromptCheckBox->setChecked(mTool->isDoPrompt());

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ReplaceConfigGroupId);

    const Okteta::ByteArrayComboBox::Coding searchDataCoding = configGroup.readEntry(SearchDataCodingConfigKey, DefaultSearchDataCoding);
    setSearchDataCoding(searchDataCoding);

    const Okteta::ByteArrayComboBox::Coding replaceDataCoding = configGroup.readEntry(ReplaceDataCodingConfigKey, DefaultReplaceDataCoding);
    setReplaceDataCoding(replaceDataCoding);

    const bool fromCursor = configGroup.readEntry(FromCursorConfigKey, DefaultFromCursor);
    setFromCursor(fromCursor);

    const Kasten::FindDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    setDirection(direction);
}

ReplaceDialog::~ReplaceDialog() = default;

QByteArray ReplaceDialog::replaceData()  const
{
    return ReplaceDataEdit->byteArray();
}

bool ReplaceDialog::prompt() const
{
    return PromptCheckBox->isChecked();
}

Okteta::ByteArrayComboBox::Coding ReplaceDialog::replaceDataCoding() const
{
    return static_cast<Okteta::ByteArrayComboBox::Coding>(ReplaceDataEdit->format());
}

void ReplaceDialog::setReplaceDataCoding(Okteta::ByteArrayComboBox::Coding replaceDataCoding)
{
    ReplaceDataEdit->setFormat(replaceDataCoding);
}

void ReplaceDialog::setCharCodec(const QString& codecName)
{
    ReplaceDataEdit->setCharCodec(codecName);
    AbstractFindDialog::setCharCodec(codecName);
}

void ReplaceDialog::onFindButtonClicked()
{
    hide();

    rememberCurrentSettings();

    mTool->setSearchData(searchData());
    mTool->setReplaceData(replaceData());
    mTool->setCaseSensitivity(caseSensitivity());
    mTool->setDoPrompt(prompt());

    mTool->replace(direction(), fromCursor(), inSelection());
}

void ReplaceDialog::showEvent(QShowEvent* showEvent)
{
    AbstractFindDialog::showEvent(showEvent);

    setInSelectionEnabled(mTool->hasSelectedData());
    setInSelection(mTool->hasSelectedData());
    setCharCodec(mTool->charCodingName());
}

void ReplaceDialog::rememberCurrentSettings()
{
    AbstractFindDialog::rememberCurrentSettings();

    ReplaceDataEdit->rememberCurrentByteArray();

    KConfigGroup configGroup(KSharedConfig::openConfig(), ReplaceConfigGroupId);
    configGroup.writeEntry(SearchDataCodingConfigKey, searchDataCoding());
    configGroup.writeEntry(ReplaceDataCodingConfigKey, replaceDataCoding());
    configGroup.writeEntry(DirectionConfigKey, direction());
    configGroup.writeEntry(FromCursorConfigKey, fromCursor());
}

}

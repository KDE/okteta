/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchdialog.hpp"

// controller
#include "searchtool.hpp"
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
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
static constexpr Okteta::ByteArrayComboBox::Coding DefaultSearchDataCoding = Okteta::ByteArrayComboBox::HexadecimalCoding;

static constexpr char SearchConfigGroupId[] = "SearchTool";

static constexpr char FromCursorConfigKey[] = "FromCursor";
static constexpr char DirectionConfigKey[] = "Direction";
static constexpr char SearchDataCodingConfigKey[] = "SearchDataCoding";

namespace Kasten {

SearchDialog::SearchDialog(SearchTool* tool, QWidget* parent)
    : AbstractFindDialog(parent)
    , mTool(tool)
{
    setWindowTitle(i18nc("@title:window", "Find Bytes"));

    setFindButton(i18nc("@action:button", "&Find"),
                  QStringLiteral("edit-find"),
                  i18nc("@info:tooltip", "Start searching"),
                  xi18nc("@info:whatsthis",
                         "If you press the <interface>Find</interface> button, "
                         "the bytes you entered above are searched for within "
                         "the byte array."));

    setupFindBox();
    setupOperationBox();
    setupCheckBoxes();

    setFindButtonEnabled(false);
    setModal(true);

    //
    setCaseSensitivity(mTool->caseSensitivity());

    const KConfigGroup configGroup(KSharedConfig::openConfig(), SearchConfigGroupId);

    const Okteta::ByteArrayComboBox::Coding searchDataCoding = configGroup.readEntry(SearchDataCodingConfigKey, DefaultSearchDataCoding);
    setSearchDataCoding(searchDataCoding);

    const bool fromCursor = configGroup.readEntry(FromCursorConfigKey, DefaultFromCursor);
    setFromCursor(fromCursor);

    const Kasten::FindDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    setDirection(direction);

    setCharCodec(mTool->charCodingName());
    connect(mTool,  &SearchTool::charCodecChanged,
            this, &SearchDialog::setCharCodec);
}

SearchDialog::~SearchDialog() = default;

void SearchDialog::onFindButtonClicked()
{
    hide();

    rememberCurrentSettings();

    mTool->setSearchData(searchData());
    mTool->setCaseSensitivity(caseSensitivity());

    mTool->search(direction(), fromCursor(), inSelection());
}

void SearchDialog::showEvent(QShowEvent* showEvent)
{
    AbstractFindDialog::showEvent(showEvent);

    setInSelectionEnabled(mTool->hasSelectedData());
    setInSelection(mTool->hasSelectedData());
}

void SearchDialog::rememberCurrentSettings()
{
    AbstractFindDialog::rememberCurrentSettings();

    KConfigGroup configGroup(KSharedConfig::openConfig(), SearchConfigGroupId);
    configGroup.writeEntry(SearchDataCodingConfigKey, searchDataCoding());
    configGroup.writeEntry(DirectionConfigKey, direction());
    configGroup.writeEntry(FromCursorConfigKey, fromCursor());
}

}

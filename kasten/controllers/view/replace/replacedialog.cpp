/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacedialog.hpp"

// controller
#include "replacetool.hpp"
// libconfigentries
#include <bytearraycomboboxcodingconfigentry.hpp>
#include <finddirectionconfigentry.hpp>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>


namespace Kasten {

// C++11 needs a definition for static constexpr members
constexpr char ReplaceDialog::ConfigGroupId[];

constexpr char ReplaceDialog::FromCursorConfigKey[];
constexpr char ReplaceDialog::DirectionConfigKey[];
constexpr char ReplaceDialog::InSelectionConfigKey[];
constexpr char ReplaceDialog::ReplaceDataCodingConfigKey[];
constexpr char ReplaceDialog::SearchDataCodingConfigKey[];

constexpr bool ReplaceDialog::DefaultFromCursor;
constexpr Kasten::FindDirection ReplaceDialog::DefaultDirection;
constexpr bool ReplaceDialog::DefaultInSelection;
constexpr Okteta::ByteArrayComboBox::Coding ReplaceDialog::DefaultReplaceDataCoding;
constexpr Okteta::ByteArrayComboBox::Coding ReplaceDialog::DefaultSearchDataCoding;


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
    auto* ReplaceBox = new QGroupBox(i18nc("@title:group", "Replace With"));

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

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    const Okteta::ByteArrayComboBox::Coding searchDataCoding = configGroup.readEntry(SearchDataCodingConfigKey, DefaultSearchDataCoding);
    setSearchDataCoding(searchDataCoding);

    const Okteta::ByteArrayComboBox::Coding replaceDataCoding = configGroup.readEntry(ReplaceDataCodingConfigKey, DefaultReplaceDataCoding);
    setReplaceDataCoding(replaceDataCoding);

    const bool fromCursor = configGroup.readEntry(FromCursorConfigKey, DefaultFromCursor);
    setFromCursor(fromCursor);

    const Kasten::FindDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    setDirection(direction);

    const bool inSelection = configGroup.readEntry(InSelectionConfigKey, DefaultInSelection);
    setInSelection(inSelection);
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
    // TODO: find a smart heuristic when to automatically set this flag
    // also when to use the content of the current selection as initial search pattern
    // setInSelection(mTool->hasSelectedData());
    setCharCodec(mTool->charCodingName());
}

void ReplaceDialog::rememberCurrentSettings()
{
    AbstractFindDialog::rememberCurrentSettings();

    ReplaceDataEdit->rememberCurrentByteArray();

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(SearchDataCodingConfigKey, searchDataCoding());
    configGroup.writeEntry(ReplaceDataCodingConfigKey, replaceDataCoding());
    configGroup.writeEntry(DirectionConfigKey, direction());
    configGroup.writeEntry(FromCursorConfigKey, fromCursor());
    configGroup.writeEntry(InSelectionConfigKey, persistentInSelection());
}

}

#include "moc_replacedialog.cpp"

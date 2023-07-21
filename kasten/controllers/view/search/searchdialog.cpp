/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009, 2023 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchdialog.hpp"

// controller
#include "searchtool.hpp"
// libconfigentries
#include <bytearraycomboboxcodingconfigentry.hpp>
#include <finddirectionconfigentry.hpp>
// KF
#include <KConfigGroup>
#include <KSharedConfig>
#include <KLocalizedString>


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

    const KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);

    const Okteta::ByteArrayComboBox::Coding searchDataCoding = configGroup.readEntry(SearchDataCodingConfigKey, DefaultSearchDataCoding);
    setSearchDataCoding(searchDataCoding);

    const bool fromCursor = configGroup.readEntry(FromCursorConfigKey, DefaultFromCursor);
    setFromCursor(fromCursor);

    const Kasten::FindDirection direction = configGroup.readEntry(DirectionConfigKey, DefaultDirection);
    setDirection(direction);

    const bool inSelection = configGroup.readEntry(InSelectionConfigKey, DefaultInSelection);
    setInSelection(inSelection);

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
    // TODO: find a smart heuristic when to automatically set this flag
    // also when to use the content of the current selection as initial search pattern
    // setInSelection(mTool->hasSelectedData());
}

void SearchDialog::rememberCurrentSettings()
{
    AbstractFindDialog::rememberCurrentSettings();

    KConfigGroup configGroup(KSharedConfig::openConfig(), ConfigGroupId);
    configGroup.writeEntry(SearchDataCodingConfigKey, searchDataCoding());
    configGroup.writeEntry(DirectionConfigKey, direction());
    configGroup.writeEntry(FromCursorConfigKey, fromCursor());
    configGroup.writeEntry(InSelectionConfigKey, persistentInSelection());
}

}

#include "moc_searchdialog.cpp"

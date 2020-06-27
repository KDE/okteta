/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "searchdialog.hpp"

// controller
#include "searchtool.hpp"
// KF
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

    setCharCodec(mTool->charCodingName());
    connect(mTool,  &SearchTool::charCodecChanged,
            this, &SearchDialog::setCharCodec);
}

SearchDialog::~SearchDialog() = default;

void SearchDialog::onFindButtonClicked()
{
    hide();

    rememberCurrentSettings();

    mTool->setSearchData(data());
    mTool->setCaseSensitivity(caseSensitivity());

    mTool->search(direction(), fromCursor(), inSelection());
}

void SearchDialog::showEvent(QShowEvent* showEvent)
{
    AbstractFindDialog::showEvent(showEvent);

    setInSelectionEnabled(mTool->hasSelectedData());
    setInSelection(mTool->hasSelectedData());
}

}

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "replacedialog.hpp"

// controller
#include "replacetool.hpp"
// Okteta Kasten gui
#include <Kasten/Okteta/ByteArrayComboBox>
// KF
#include <KLocalizedString>
// Qt
#include <QCheckBox>
#include <QGroupBox>
#include <QLayout>
#include <QPushButton>

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

void ReplaceDialog::setCharCodec(const QString& codecName)
{
    ReplaceDataEdit->setCharCodec(codecName);
    AbstractFindDialog::setCharCodec(codecName);
}

void ReplaceDialog::onFindButtonClicked()
{
    hide();

    rememberCurrentSettings();

    mTool->setSearchData(data());
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
}

}

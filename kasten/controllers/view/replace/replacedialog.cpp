/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
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

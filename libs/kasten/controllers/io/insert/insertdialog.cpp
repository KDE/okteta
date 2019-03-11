/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009,2013 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "insertdialog.hpp"

// Kasten gui
#include <Kasten/AbstractModelDataGeneratorConfigEditor>
// KF
#include <KLocalizedString>
// Qt
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QDialogButtonBox>

namespace Kasten {

InsertDialog::InsertDialog(AbstractModelDataGeneratorConfigEditor* configEditor, QWidget* parent)
    : QDialog(parent)
    , mConfigEditor(configEditor)
{
    setWindowTitle(i18nc("@title:window", "Insert"));

    // editor
    QLabel* editorLabel = new QLabel(mConfigEditor->name());
    QFont font = editorLabel->font();
    font.setBold(true);
    editorLabel->setFont(font);

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    QPushButton* insertButton = new QPushButton(i18nc("@action:button", "&Insert"));
    insertButton->setToolTip(i18nc("@info:tooltip",
                                   "Insert the generated data into the document."));
    insertButton->setWhatsThis(xi18nc("@info:whatsthis",
                                      "If you press the <interface>Insert</interface> button, "
                                      "the data will be generated with the settings you entered above "
                                      "and inserted into the document at the cursor position."));

    dialogButtonBox->addButton(insertButton, QDialogButtonBox::AcceptRole);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    insertButton->setEnabled(configEditor->isValid());
    connect(configEditor, &AbstractModelDataGeneratorConfigEditor::validityChanged,
            insertButton, &QWidget::setEnabled);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(editorLabel);
    layout->addWidget(mConfigEditor);
    layout->addStretch();
    layout->addWidget(dialogButtonBox);

    setLayout(layout);
}

InsertDialog::~InsertDialog() = default;

}

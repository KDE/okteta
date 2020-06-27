/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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

InsertDialog::InsertDialog(AbstractModelDataGeneratorConfigEditor* configEditor,
                           AbstractModelDataGenerator* generator,
                           QWidget* parent)
    : QDialog(parent)
    , mConfigEditor(configEditor)
    , m_generator(generator)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    mConfigEditor->setParent(this);

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

    connect(this, &QDialog::finished, this, &InsertDialog::onFinished);
}

InsertDialog::~InsertDialog() = default;

void InsertDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit insertAccepted(m_generator);
}

}

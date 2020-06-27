/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2009, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "createdialog.hpp"

// Kasten gui
#include <abstractmodeldatageneratorconfigeditor.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QDialogButtonBox>

namespace Kasten {

CreateDialog::CreateDialog(AbstractModelDataGeneratorConfigEditor* configEditor,
                           AbstractModelDataGenerator* generator,
                           QWidget* parent)
    : QDialog(parent)
    , mConfigEditor(configEditor)
    , m_generator(generator)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    mConfigEditor->setParent(this);

    setWindowTitle(i18nc("@title:window", "Create"));

    // editor
    QLabel* editorLabel = new QLabel(mConfigEditor->name());
    QFont font = editorLabel->font();
    font.setBold(true);
    editorLabel->setFont(font);

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    QPushButton* createButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-new")),
                                                i18nc("@action:button create the new document", "&Create"));
    createButton->setToolTip(i18nc("@info:tooltip",
                                   "Create a new document with the generated data."));
    createButton->setWhatsThis(xi18nc("@info:whatsthis",
                                      "If you press the <interface>Create</interface> button, "
                                      "the data will be generated with the settings you entered above "
                                      "and inserted in a new document."));

    dialogButtonBox->addButton(createButton, QDialogButtonBox::AcceptRole);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    createButton->setEnabled(configEditor->isValid());
    connect(configEditor, &AbstractModelDataGeneratorConfigEditor::validityChanged,
            createButton, &QWidget::setEnabled);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(editorLabel);
    layout->addWidget(mConfigEditor);
    layout->addStretch();
    layout->addWidget(dialogButtonBox);

    setLayout(layout);

    connect(this, &QDialog::finished, this, &CreateDialog::onFinished);
}

CreateDialog::~CreateDialog() = default;

void CreateDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit createAccepted(m_generator);
}

}

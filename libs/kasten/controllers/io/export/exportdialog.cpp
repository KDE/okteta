/*
    This file is part of the Kasten Framework, made within the KDE community.

    SPDX-FileCopyrightText: 2008, 2013 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "exportdialog.hpp"

// Kasten gui
#include <Kasten/AbstractModelExporterConfigEditor>
#include <Kasten/AbstractSelectionView>
// KF
#include <KLocalizedString>
// Qt
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QDialogButtonBox>

namespace Kasten {

ExportDialog::ExportDialog(const QString& remoteTypeName,
                           AbstractModelExporterConfigEditor* configEditor,
                           AbstractModelExporter* exporter,
                           QWidget* parent)
    : QDialog(parent)
    , mConfigEditor(configEditor)
    , m_exporter(exporter)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    mConfigEditor->setParent(this);

    setWindowTitle(i18nc("@title:window", "Export"));

    auto* splitter = new QSplitter(this);

    // config editor
    QWidget* editorPage = new QWidget(splitter);
    auto* editorPageLayout = new QVBoxLayout(editorPage);
    QLabel* editorLabel = new QLabel(remoteTypeName);
    QFont font = editorLabel->font();
    font.setBold(true);
    editorLabel->setFont(font);

    editorPageLayout->addWidget(editorLabel);
    editorPageLayout->addWidget(mConfigEditor);
    editorPageLayout->addStretch();

    splitter->addWidget(editorPage);
    splitter->setCollapsible(0, false);

    mPreviewView = configEditor->createPreviewView();

    if (mPreviewView) {
        QGroupBox* previewBox = new QGroupBox(i18nc("@title:group", "Preview"), this);
        splitter->addWidget(previewBox);

        auto* previewBoxLayout = new QHBoxLayout(previewBox);

        previewBoxLayout->addWidget(mPreviewView->widget());
    }

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    QPushButton* exportButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-export")),
                                                i18nc("@action:button", "&Export to File..."));
    exportButton->setToolTip(i18nc("@info:tooltip",
                                   "Export the selected data to a file."));
    exportButton->setWhatsThis(xi18nc("@info:whatsthis",
                                      "If you press the <interface>Export to file</interface> "
                                      "button, the selected data will be copied to a file "
                                      "with the settings you entered above."));

    dialogButtonBox->addButton(exportButton, QDialogButtonBox::AcceptRole);
    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    dialogButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    exportButton->setEnabled(configEditor->isValid());
    connect(configEditor, &AbstractModelExporterConfigEditor::validityChanged,
            exportButton, &QWidget::setEnabled);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(splitter);
    layout->addStretch();
    layout->addWidget(dialogButtonBox);

    setLayout(layout);

    connect(this, &QDialog::finished, this, &ExportDialog::onFinished);
}

ExportDialog::~ExportDialog()
{
    delete mPreviewView;
}

void ExportDialog::setData(AbstractModel* model, const AbstractModelSelection* selection)
{
    m_selection = selection;

    if (mPreviewView) {
        mPreviewView->setData(model, selection);
    }
}

void ExportDialog::onFinished(int result)
{
    if (result != QDialog::Accepted) {
        return;
    }

    emit exportAccepted(m_exporter, m_selection);
}

}

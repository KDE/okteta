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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QFont>
#include <QDialogButtonBox>

namespace Kasten {

ExportDialog::ExportDialog(const QString& remoteTypeName,
                           std::unique_ptr<AbstractModelExporterConfigEditor>&& configEditor,
                           AbstractModelExporter* exporter,
                           QWidget* parent)
    : QDialog(parent)
    , mConfigEditor(configEditor.release()) // to be life-time handled by QWidget parentship
    , m_exporter(exporter)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    mConfigEditor->setParent(this);

    setWindowTitle(i18nc("@title:window", "Export"));

    auto* splitter = new QSplitter(this);

    // config editor
    auto* editorPage = new QWidget(splitter);
    auto* editorPageLayout = new QVBoxLayout(editorPage);
    auto* editorLabel = new QLabel(remoteTypeName);
    QFont font = editorLabel->font();
    font.setBold(true);
    editorLabel->setFont(font);

    editorPageLayout->addWidget(editorLabel);
    editorPageLayout->addWidget(mConfigEditor);
    editorPageLayout->addStretch();

    splitter->addWidget(editorPage);
    splitter->setCollapsible(0, false);

    mPreviewView = mConfigEditor->createPreviewView();

    if (mPreviewView) {
        auto* previewBox = new QGroupBox(i18nc("@title:group", "Preview"), this);
        splitter->addWidget(previewBox);

        auto* previewBoxLayout = new QHBoxLayout(previewBox);

        previewBoxLayout->addWidget(mPreviewView->widget());
    }

    // dialog buttons
    auto* dialogButtonBox = new QDialogButtonBox;
    auto* exportButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-export")),
                                         i18nc("@action:button", "&Export to File…"));
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

    exportButton->setEnabled(mConfigEditor->isValid());
    connect(mConfigEditor, &AbstractModelExporterConfigEditor::validityChanged,
            exportButton, &QWidget::setEnabled);

    // main layout
    auto* layout = new QVBoxLayout;
    layout->addWidget(splitter, 1);
    layout->addWidget(dialogButtonBox);

    setLayout(layout);

    connect(this, &QDialog::finished, this, &ExportDialog::onFinished);
}

ExportDialog::~ExportDialog() = default;

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

    Q_EMIT exportAccepted(m_exporter, m_selection);
}

}

#include "moc_exportdialog.cpp"

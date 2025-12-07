/*
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
    SPDX-FileCopyrightText: 2009, 2012 Alex Richardson <alex.richardson@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structuresmanagerview.hpp"

// controller
#include "structuresselectiondialog.hpp"
#include "structuresselector.hpp"
#include <structuresmanager.hpp>
#include <structureinstalljob.hpp>
#include <structureuninstalljob.hpp>
#include <structurestool.hpp>
#include <structureslogging.hpp>
// KF
#include <KConfigDialogManager>
#include <KLocalizedString>
#include <KNSWidgets/Button>
#include <KStandardGuiItem>
#include <KMessageBox>
// Qt
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QMimeData>

namespace Kasten {

StructuresManagerView::StructuresManagerView(StructuresTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* const pageLayout = new QVBoxLayout();
    pageLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pageLayout);

    mStructuresSelector = new StructuresSelector(this);
    connect(mStructuresSelector, &StructuresSelector::enabledStructuresChanged,
            this, &StructuresManagerView::changed);
    connect(mStructuresSelector, &StructuresSelector::uninstallStructureRequested,
            this, &StructuresManagerView::uninstallStructure);
    connect(mStructuresSelector, &StructuresSelector::dataOffered,
            this, &StructuresManagerView::onDataOffered);
    connect(mStructuresSelector, &StructuresSelector::dataDropped,
            this, &StructuresManagerView::onDataDropped);

    pageLayout->addWidget(mStructuresSelector);

    // buttons
    auto* const buttonsLayout = new QHBoxLayout();

    buttonsLayout->addStretch();

    mAdvancedSelectionButton = new QPushButton(QIcon::fromTheme(QStringLiteral("configure")), i18nc("@action:button", "Advanced Selection…"), this);
    connect(mAdvancedSelectionButton, &QPushButton::clicked, this, &StructuresManagerView::advancedSelection);
    buttonsLayout->addWidget(mAdvancedSelectionButton);

    m_installStructureButton = new QPushButton(QIcon::fromTheme(QStringLiteral("document-import")), i18nc("@action:button", "Install from File(s)…"), this);
    connect(m_installStructureButton, &QPushButton::clicked, this, &StructuresManagerView::selectStructureFile);
    buttonsLayout->addWidget(m_installStructureButton);

    mGetNewStructuresButton = new KNSWidgets::Button(i18nc("@action:button", "Get New Structures…"),
                                                     QStringLiteral("okteta-structures.knsrc"), this);
    connect(mGetNewStructuresButton, &KNSWidgets::Button::dialogFinished,
            this, &StructuresManagerView::onGetNewStructuresClicked);
    buttonsLayout->addWidget(mGetNewStructuresButton);

    pageLayout->addLayout(buttonsLayout);

    mStructuresSelector->setStructures(mTool->manager()->structureDefs());
}

StructuresManagerView::~StructuresManagerView() = default;

void StructuresManagerView::onGetNewStructuresClicked(const QList<KNSCore::Entry>& changedEntries)
{
    for (const KNSCore::Entry& e : changedEntries) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Changed Entry: " << e.name();
        if (e.status() == KNSCore::Entry::Installed) {
            // new element installed
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed files:" << e.installedFiles();
        }
        if (e.status() == KNSCore::Entry::Deleted) {
            // element uninstalled
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "deleted files:" << e.uninstalledFiles();
        }
    }

    if (!changedEntries.isEmpty()) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed structures changed ->  rebuilding list of installed structures";
        mTool->manager()->reloadPaths();
        resetLoadedStructures();
    }
}

void StructuresManagerView::selectStructureFile()
{
    auto* const dialog = new QFileDialog(QApplication::activeWindow());
    dialog->setAttribute(Qt::WA_DeleteOnClose, true);
    dialog->setFileMode(QFileDialog::ExistingFiles);
    dialog->setMimeTypeFilters(StructuresManager::installableMimeTypes());
    connect(dialog, &QFileDialog::urlsSelected, this, &StructuresManagerView::installStructuresFromFiles);
    dialog->open();
}

void StructuresManagerView::installStructuresFromFiles(const QList<QUrl>& structureFileUrls)
{
    bool anySuccess = false;
    // cache the last installed structure id and focus it in the view
    QString id;
    for (const auto& structureFileUrl : structureFileUrls) {
        StructureInstallJob* const installJob = mTool->manager()->installStructureFromFile(structureFileUrl);

        // TODO: make async
        const bool success = installJob->exec();

        if (success) {
            anySuccess = true;
            id = installJob->structureId();
        } else {
            KMessageBox::error(this,
                xi18n("Installing from <filename>%1</filename> failed:\n\n%2", structureFileUrl.toDisplayString(QUrl::PreferLocalFile), installJob->errorString()),
                i18nc("@title:window", "Structure Installation"),
                KMessageBox::Notify
            );
        }
    }
    if (anySuccess) {
        resetLoadedStructures();
        // TODO: only focus, or highlight or display a message about them?
        mStructuresSelector->selectAndFocusStructure(id);
    }
}

void StructuresManagerView::uninstallStructure(const QString& id)
{
    StructureDefinitionFile* const definition = mTool->manager()->definition(id);

    if (!definition) {
        // TODO: warning, should not happen
        return;
    }

    const QString structureName = definition->metaData().name();

    const KGuiItem uninstallGuiItem(i18nc("@action:button", "Uninstall"), QStringLiteral("edit-delete"));

    const int remove = KMessageBox::warningContinueCancel(this,
        i18n("Do you want to uninstall \"%1\"?", structureName),
        i18nc("@title:window", "Structure Uninstallation"),
        uninstallGuiItem,KStandardGuiItem::cancel(),
        QStringLiteral("warningUninstallStructure")
    );

    if (remove != KMessageBox::Continue) {
        return;
    }

    StructureUninstallJob* const uninstallJob = mTool->manager()->uninstallStructure(id);

    // TODO: make async
    const bool success = uninstallJob->exec();

    if (success) {
        resetLoadedStructures();
    } else {
        KMessageBox::error(this,
            uninstallJob->errorString(),
            i18nc("@title:window", "Structure Uninstallation"),
            KMessageBox::Notify
        );
    }
}

QStringList StructuresManagerView::values() const
{
    return mStructuresSelector->enabledStructures();;
}

void StructuresManagerView::setValues(const QStringList& values)
{
    mStructuresSelector->setEnabledStructures(values);
}

void StructuresManagerView::advancedSelection()
{
    auto* const dialog = new StructuresSelectionDialog(mTool->manager()->structureDefs(),
                                                       mStructuresSelector->enabledList(),
                                                       this);
    connect(dialog, &StructuresSelectionDialog::structuresAccepted,
            this, &StructuresManagerView::setEnabledStructures);
    dialog->open();
}

void StructuresManagerView::setEnabledStructures(const QStringList& enabledStructures)
{
    mStructuresSelector->setEnabledStructures(enabledStructures);;
}

void StructuresManagerView::resetLoadedStructures()
{
    mStructuresSelector->setStructures(mTool->manager()->structureDefs());
}

void StructuresManagerView::onDataOffered(const QMimeData* mimeData, bool& isAccepted)
{
    isAccepted = mimeData->hasUrls();
}

void StructuresManagerView::onDataDropped(const QMimeData* mimeData)
{
    const QList<QUrl> urls = mimeData->urls();

    installStructuresFromFiles(urls);
}

}

#include "moc_structuresmanagerview.cpp"

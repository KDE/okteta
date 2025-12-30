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
#include <structurestool.hpp>
#include <structureslogging.hpp>
// KF
#include <KConfigDialogManager>
#include <KLocalizedString>
#include <KNSWidgets/Button>
// Qt
#include <QPushButton>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>

StructuresManagerView::StructuresManagerView(Kasten::StructuresTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* const pageLayout = new QVBoxLayout();
    pageLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pageLayout);

    mStructuresSelector = new StructuresSelector(this);
    connect(mStructuresSelector, &StructuresSelector::enabledStructuresChanged,
            this, &StructuresManagerView::changed);

    pageLayout->addWidget(mStructuresSelector);

    // buttons
    auto* const buttonsLayout = new QHBoxLayout();

    buttonsLayout->addStretch();

    mAdvancedSelectionButton = new QPushButton(QIcon::fromTheme(QStringLiteral("configure")), i18nc("@action:button", "Advanced Selection…"), this);
    connect(mAdvancedSelectionButton, &QPushButton::clicked, this, &StructuresManagerView::advancedSelection);
    buttonsLayout->addWidget(mAdvancedSelectionButton);

    mGetNewStructuresButton = new KNSWidgets::Button(i18nc("@action:button", "Get New Structures…"),
                                                     QStringLiteral("okteta-structures.knsrc"), this);
    connect(mGetNewStructuresButton, &KNSWidgets::Button::dialogFinished,
            this, &StructuresManagerView::onGetNewStructuresClicked);
    buttonsLayout->addWidget(mGetNewStructuresButton);
}

StructuresManagerView::~StructuresManagerView() = default;

void StructuresManagerView::onGetNewStructuresClicked(const QList<KNSCore::EntryInternal>& changedEntries)
{
    for (const KNSCore::EntryInternal& e : changedEntries) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Changed Entry: " << e.name();
        if (e.status() == KNS3::Entry::Installed) {
            // new element installed
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed files:" << e.installedFiles();
        }
        if (e.status() == KNS3::Entry::Deleted) {
            // element uninstalled
            qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "deleted files:" << e.uninstalledFiles();
        }
    }

    if (!changedEntries.isEmpty()) {
        qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "installed structures changed ->  rebuilding list of installed structures";
        mTool->manager()->reloadPaths();
        mStructuresSelector->setStructures(mTool->manager()->structureDefs());
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

#include "moc_structuresmanagerview.cpp"

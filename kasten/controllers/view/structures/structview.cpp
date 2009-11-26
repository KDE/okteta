/*
 *   This file is part of the Okteta Kasten module, part of the KDE project.
 *
 *   Copyright 2009 Alex Richardson <alex.richardson@gmx.de>
 *   Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include "structview.h"

// controller
#include "structtreemodel.h"
#include "structtool.h"
#include "structviewitemdelegate.h"
//settings
#include "structviewpreferences.h"
#include "settings/structviewdisplaysettingswidget.h"
#include "settings/structuresmanagerview.h"
#include "settings/structureaddremovewidget.h"
// KDE
#include <KComboBox>
#include <KLocale>
#include <KConfigDialog>
#include <KPushButton>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QTreeView>
#include <QtGui/QHeaderView>
#include <QFocusEvent>

//TODO add bool parameter to read data to prevent frequent updating of treeview
namespace Kasten
{

StructView::StructView(StructTool* tool, QWidget* parent) :
    QWidget(parent), mTool(tool), mDelegate(new StructViewItemDelegate(this))
{
    QBoxLayout* baseLayout = new QVBoxLayout(this);
    setLayout(baseLayout);
    baseLayout->setMargin(0);
    // table
    mStructTreeModel = new StructTreeModel(mTool, this);
    mStructTreeView = new QTreeView(this);
    mStructTreeView->setObjectName("StructTree");
    mStructTreeView->setRootIsDecorated(true);
    mStructTreeView->setAlternatingRowColors(true);
    mStructTreeView->setItemsExpandable(true);
    mStructTreeView->setUniformRowHeights(true);
    mStructTreeView->setAllColumnsShowFocus(true);
    mStructTreeView->setEditTriggers(QAbstractItemView::DoubleClicked
            | QAbstractItemView::EditKeyPressed);
    mStructTreeView->setItemDelegate(mDelegate);
    mStructTreeView->setDragEnabled(false);
    mStructTreeView->setSortingEnabled(false);
    mStructTreeView->setModel(mStructTreeModel);
    mStructTreeView->setHeaderHidden(false);
    mStructTreeView->setSortingEnabled(false);
    mStructTreeView->installEventFilter(this);
    QHeaderView* header = mStructTreeView->header();
    header->setResizeMode(QHeaderView::Interactive);

    baseLayout->addWidget(mStructTreeView, 10);

    // settings
    QBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->setMargin(0);

    mByteOrderSelection = new KComboBox(this);
    mByteOrderSelection->addItem(i18nc("@item:inlistbox", "Little-endian")); // add first for index
    mByteOrderSelection->addItem(i18nc("@item:inlistbox", "Big-endian")); // add second for index
    mByteOrderSelection->setCurrentIndex(mTool->byteOrder());
    connect(mByteOrderSelection, SIGNAL(activated( int )), mTool,
            SLOT(setByteOrder(int)));
    const QString byteOrderToolTip = i18nc("@info:tooltip",
            "The byte order used to decode the values.");
    mByteOrderSelection->setToolTip(byteOrderToolTip);
    settingsLayout->addWidget(mByteOrderSelection);
    settingsLayout->addStretch();
    baseLayout->addLayout(settingsLayout);

    KIcon settings = KIcon("configure");
    mSettingsButton = new KPushButton(settings, i18n("Settings"), this);
    const QString settingsTooltip = i18nc("@info:tooltip", "Open settings.");
    mSettingsButton->setToolTip(settingsTooltip);
    connect(mSettingsButton, SIGNAL(pressed()), this, SLOT(openSettingsDlg()));
    settingsLayout->addWidget(mSettingsButton);
    connect(mStructTreeView->selectionModel(),
            SIGNAL(currentRowChanged( const QModelIndex&, const QModelIndex& )),
            SLOT(onCurrentRowChanged( const QModelIndex&, const QModelIndex& )));
}

void StructView::openSettingsDlg(int page)
{
    //An instance of your dialog could be already created and could be cached,
    //in which case you want to display the cached dialog instead of creating
    //another one
    if (KConfigDialog::showDialog("Structures Tool Settings"))
        return;

    //KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog* dialog = new KConfigDialog(this, "Structures Tool Settings",
            StructViewPreferences::self());
    QWidget* displaySettings = new StructViewDisplaySettingsWidget();
    QWidget* structureSettings = new StructuresManagerView(&mTool->manager(), this);
    QWidget* loadedStructuresSettings = new StructureAddRemoveWidget(mTool, this);
    KPageWidgetItem* displ = dialog->addPage(displaySettings, i18n("Value Display"),
            QString("configure"));
    QWidget* structsWidget = new QWidget();
    {
        QVBoxLayout* layout = new QVBoxLayout();
        loadedStructuresSettings->setObjectName("kcfg_LoadedStructures");
        layout->addWidget(loadedStructuresSettings);
        structsWidget->setLayout(layout);
    }
    KPageWidgetItem* structs = dialog->addPage(structsWidget,
            i18n("Structures"), "configure");

     dialog->addPage(structureSettings, i18n("Structures management"), "folder");

    //User edited the configuration - update your local copies of the
    //configuration data
    connect(dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(update()));
     connect(structureSettings, SIGNAL(applyButtonClicked()),
            loadedStructuresSettings, SLOT(updateAvailable()));
    connect(dialog, SIGNAL(settingsChanged(const QString&)), mTool,
            SLOT(loadStructDefFiles()));
    connect(dialog, SIGNAL(settingsChanged(const QString&)), mTool,
            SLOT(setSelectedStructuresInView()));
    connect(dialog, SIGNAL(settingsChanged(const QString&)),
            loadedStructuresSettings, SLOT(updateAvailable()));
    //TODO kconfig_compiler signals not working (or am I doing it wrong?)
    if (page == 0)
        dialog->setCurrentPage(displ);
    else if (page == 1)
        dialog->setCurrentPage(structs);

    dialog->show();
}

bool StructView::eventFilter(QObject* object, QEvent* event)
{
    if (object == mStructTreeView)
    {
        if (event->type() == QEvent::FocusIn)
        {
            QFocusEvent* focusEvent = static_cast<QFocusEvent*> (event);
            const Qt::FocusReason focusReason = focusEvent->reason();
            if (focusReason != Qt::ActiveWindowFocusReason && focusReason
                    != Qt::PopupFocusReason)
            {
                const QModelIndex current =
                        mStructTreeView->selectionModel()->currentIndex();
                kDebug() << "current index: " << current;
                if (current.isValid())
                    mTool->mark(current);
                else
                    mTool->unmark();
            }
        }
        else if (event->type() == QEvent::FocusOut)
        {
            QFocusEvent* focusEvent = static_cast<QFocusEvent*> (event);
            const Qt::FocusReason focusReason = focusEvent->reason();
            if (focusReason != Qt::ActiveWindowFocusReason && focusReason
                    != Qt::PopupFocusReason)
                mTool->unmark();
        }
    }
    return QWidget::eventFilter(object, event);
}

void StructView::onCurrentRowChanged(const QModelIndex& current,
        const QModelIndex& previous)
{
    Q_UNUSED( previous )
    if (current.isValid())
        mTool->mark(current);
    else
        mTool->unmark();
}

StructView::~StructView()
{
}
}

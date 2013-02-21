/*
 *   This file is part of the Okteta Kasten Framework, made within the KDE community.
 *
 *   Copyright 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
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
#include "structuresmanager.h"
#include "structviewitemdelegate.h"
//settings
#include "structviewpreferences.h"
#include "settings/structviewdisplaysettingswidget.h"
#include "settings/structuresmanagerview.h"
#include "settings/structureaddremovewidget.h"

#include "script/scriptutils.h"
#include "script/scriptloggerview.h"

//#include "modeltest.h"

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

namespace Kasten2
{

StructView::StructView(StructTool* tool, QWidget* parent) :
    QWidget(parent), mTool(tool), mDelegate(new StructViewItemDelegate(this)),
            mStructTreeViewFocusChild(0)
{
    QBoxLayout* baseLayout = new QVBoxLayout(this);
    setLayout(baseLayout);
    baseLayout->setMargin(0);
    // table
    mStructTreeModel = new StructTreeModel(mTool, this);
    //  mModeltest = new ModelTest(mStructTreeModel, this);
    mStructTreeView = new QTreeView(this);
    mStructTreeView->setObjectName( QLatin1String("StructTree" ));
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
    QBoxLayout* settingsLayout = new QHBoxLayout();
    settingsLayout->setMargin(0);

    baseLayout->addLayout(settingsLayout);

    KIcon validateIcon = KIcon(QLatin1String("document-sign"));
    mValidateButton = new KPushButton(validateIcon, i18nc("@action:button", "Validate"), this);
    const QString validationToolTip = i18nc("@info:tooltip", "Validate all structures.");
    mValidateButton->setToolTip(validationToolTip);
    mValidateButton->setEnabled(false); //no point validating without file open
    connect(mValidateButton, SIGNAL(clicked()), mTool, SLOT(validateAllStructures()));
    connect(mTool, SIGNAL(byteArrayModelChanged(Okteta::AbstractByteArrayModel*)),
                this, SLOT(onByteArrayModelChanged(Okteta::AbstractByteArrayModel*)));
    //TODO also disable the button if the structure has no validatable members
    settingsLayout->addWidget(mValidateButton);

    mLockStructureButton = new KPushButton(this);
    mLockStructureButton->setCheckable(true);
    setLockButtonState(false);
    mLockStructureButton->setEnabled(false); //won't work at beginning
    connect(mLockStructureButton, SIGNAL(toggled(bool)), this, SLOT(lockButtonToggled()));

    settingsLayout->addWidget(mLockStructureButton);

    settingsLayout->addStretch(); //stretch before the settings button

    KIcon console = KIcon(QLatin1String("utilities-terminal"));
    mScriptConsoleButton = new KPushButton(console, i18nc("@action:button", "Script console"), this);
    mScriptConsoleButton->setToolTip(i18nc("@info:tooltip", "Open script console."));
    connect(mScriptConsoleButton, SIGNAL(pressed()), this, SLOT(openScriptConsole()));
    settingsLayout->addWidget(mScriptConsoleButton);

    KIcon settings = KIcon(QLatin1String("configure"));
    mSettingsButton = new KPushButton(settings, i18nc("@action:button", "Settings"), this);
    const QString settingsTooltip = i18nc("@info:tooltip", "Open settings.");
    mSettingsButton->setToolTip(settingsTooltip);
    connect(mSettingsButton, SIGNAL(pressed()), this, SLOT(openSettingsDlg()));
    settingsLayout->addWidget(mSettingsButton);

    connect(mStructTreeView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));

    connect(mTool, SIGNAL(cursorIndexChanged()), SLOT(onCursorIndexChange()));
}

void StructView::onCursorIndexChange()
{
    QModelIndex idx = mStructTreeView->currentIndex();
    if (idx.isValid())
        mTool->mark(idx);
}

void StructView::openSettingsDlg(int page)
{
    //An instance of your dialog could be already created and could be cached,
    //in which case you want to display the cached dialog instead of creating
    //another one
    if (KConfigDialog::showDialog(QLatin1String("Structures Tool Settings")))
        return;

    //KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog* dialog = new KConfigDialog(this, QLatin1String("Structures Tool Settings"),
            StructViewPreferences::self());
    StructViewDisplaySettingsWidget* displaySettings = new StructViewDisplaySettingsWidget();
    QWidget* structSelectionWrapper = new QWidget();
    StructuresManagerView* structureSettings = new StructuresManagerView(mTool, this);
    KPageWidgetItem* displ = dialog->addPage(displaySettings, i18n("Value Display"),
            QLatin1String("configure"));
    QHBoxLayout* hbox = new QHBoxLayout();
    structSelectionWrapper->setLayout(hbox);
    hbox->addWidget(structureSettings);
    Q_ASSERT(structureSettings->objectName() == QLatin1String("kcfg_LoadedStructures"));
    KPageWidgetItem* management = dialog->addPage(structSelectionWrapper, i18n("Structures management"),
                                                  QLatin1String("preferences-plugin"));

    //User edited the configuration - update your local copies of the configuration data
    connect(dialog, SIGNAL(settingsChanged(QString)), mTool, SLOT(setSelectedStructuresInView()));
    connect(dialog, SIGNAL(settingsChanged(QString)), this, SLOT(update()));

    //XXX once kconfig_compiler signals work with settings dialog, use that
    if (page == 0)
        dialog->setCurrentPage(displ);
    else if (page == 1)
        dialog->setCurrentPage(management);

    dialog->show();
}

bool StructView::eventFilter(QObject* object, QEvent* event)
{
    if (object == mStructTreeView)
    {
        if (event->type() == QEvent::FocusIn)
        {
            const QModelIndex current = mStructTreeView->selectionModel()->currentIndex();

            if (current.isValid())
                mTool->mark(current);
            else
                mTool->unmark();

            //set state of lock button
            setLockButtonState(mTool->isStructureLocked(current));
            mLockStructureButton->setEnabled(mTool->canStructureBeLocked(current));
        }
        else if (event->type() == QEvent::FocusOut)
        {
            QWidget* treeViewFocusWidget = mStructTreeView->focusWidget();
            const bool subChildHasFocus = (treeViewFocusWidget != mStructTreeView);
            if (subChildHasFocus)
            {
                mStructTreeViewFocusChild = treeViewFocusWidget;
                mStructTreeViewFocusChild->installEventFilter(this);
            }
            else
                mTool->unmark();
        }
    }
    else if (object == mStructTreeViewFocusChild)
    {
        // TODO: it is only assumed the edit widget will be removed if it loses the focus
        if (event->type() == QEvent::FocusOut)
        {
            if (!mStructTreeView->hasFocus())
                mTool->unmark();
            mStructTreeViewFocusChild->removeEventFilter(this);
            mStructTreeViewFocusChild = 0;
        }
    }

    return QWidget::eventFilter(object, event);
}

void StructView::onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED( previous )
    if (current.isValid() && mTool->byteArrayModel())
    {
        mTool->mark(current);
        mLockStructureButton->setEnabled(true);
        setLockButtonState(mTool->isStructureLocked(current));
    }
    else
        mTool->unmark();
}

StructView::~StructView()
{
}

void StructView::lockButtonToggled()
{
    setLockButtonState(mLockStructureButton->isChecked());
    const QModelIndex current = mStructTreeView->selectionModel()->currentIndex();
    if (!current.isValid())
    {
        kWarning() << "it should not be possible to toggle this button when current index is invalid!";
        return;
    }

    if (mLockStructureButton->isChecked())
        mTool->lockStructure(current);
    else
        mTool->unlockStructure(current);
}

void StructView::setLockButtonState(bool structureLocked)
{
    if (structureLocked)
    {
        mLockStructureButton->setIcon(KIcon(QLatin1String("object-locked")));
        mLockStructureButton->setText(i18nc("@action:pushbutton"
                        " unlock the starting offset of the current structure", "Unlock"));
        mLockStructureButton->setToolTip(i18nc("@info:tooltip",
                        "Unlock selected structure, i.e. the starting offset is"
                        " always set to the current cursor position."));
    }
    else
    {
        mLockStructureButton->setIcon(KIcon(QLatin1String("object-unlocked")));
        mLockStructureButton->setText(i18nc("@action:pushbutton"
                        " unlock the starting offset of the current structure", "Lock"));
        mLockStructureButton->setToolTip(i18nc("@info:tooltip",
                        "Lock selected structure to current offset."));
    }
    mLockStructureButton->setChecked(structureLocked);
}

void StructView::openScriptConsole()
{
    KDialog* dialog = new KDialog(this);
    dialog->setMainWidget(new ScriptLoggerView(mTool->allData()));
    dialog->show();
}

void StructView::onByteArrayModelChanged(Okteta::AbstractByteArrayModel* model)
{
    const bool validModel = model != 0;
    QModelIndex current = mStructTreeView->currentIndex();
    mLockStructureButton->setEnabled(mTool->canStructureBeLocked(current));
    setLockButtonState(mTool->isStructureLocked(current));
    mValidateButton->setEnabled(validModel && current.isValid());
}

}

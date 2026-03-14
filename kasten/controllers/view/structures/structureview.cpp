/*
    SPDX-FileCopyrightText: 2009, 2010, 2012 Alex Richardson <alex.richardson@gmx.de>
    SPDX-FileCopyrightText: 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "structureview.hpp"

// controller
#include "structuretreemodel.hpp"
#include "structurestool.hpp"
#include "structuresmanager.hpp"
#include "structureviewitemdelegate.hpp"
#include <datainformation.hpp>
#include <scriptloggerview.hpp>
#include <structureslogging.hpp>
// settings
#include <structureviewpreferences.hpp>
#include <structureviewsettingswidget.hpp>
#include <structuresmanagerview.hpp>
#include <structureaddremovewidget.hpp>
// KF
#include <KActionMenu>
#include <KStandardAction>
#include <KLocalizedString>
#include <KConfigDialog>
// Qt
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeView>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QFocusEvent>
#include <QMenu>
#include <QToolBar>
#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QIcon>
// #include <QAbstractItemModelTester>

namespace Kasten {

StructureView::StructureView(StructuresTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
    , mDelegate(new StructureViewItemDelegate(this))
{
    // actions
    mValidateAction = new QAction(QIcon::fromTheme(QStringLiteral("document-sign")),
                                  i18nc("@action", "Validate"), this);
    mValidateAction->setToolTip(i18nc("@info:tooltip", "Validate all structures."));
    mValidateAction->setEnabled(false); // no point validating without file open
    // TODO also disable the button if the structure has no validatable members
    connect(mValidateAction, &QAction::triggered,
            mTool, &StructuresTool::validateAllStructures);

    mLockStructureAction = new QAction(this);
    mLockStructureAction->setCheckable(true);
    mLockStructureAction->setChecked(false);
    mLockStructureAction->setEnabled(false);
    onLockButtonToggled(false);
    connect(mLockStructureAction, &QAction::triggered,
            this, &StructureView::onLockButtonClicked);
    connect(mLockStructureAction, &QAction::toggled,
            this, &StructureView::onLockButtonToggled);

    mScriptConsoleAction = new QAction(QIcon::fromTheme(QStringLiteral("utilities-terminal")),
                                       i18nc("@action", "Script Console"), this);
    mScriptConsoleAction->setToolTip(i18nc("@info:tooltip", "Open script console."));
    connect(mScriptConsoleAction, &QAction::triggered,
            this, &StructureView::openScriptConsole);

    mSettingsAction = new QAction(QIcon::fromTheme(QStringLiteral("configure")),
                                  i18nc("@action", "Settings"), this);
    mSettingsAction->setToolTip(i18nc("@info:tooltip", "Open settings."));
    connect(mSettingsAction, &QAction::triggered,
            this, &StructureView::openSettingsDlg);

    m_editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")),
                               i18nc("@action:inmenu", "Edit"), this);
    connect(m_editAction, &QAction::triggered,
            this, &StructureView::editData);

    // TODO: split into explicit "Copy As Data" and "Copy As Text"
    m_copyAction = KStandardAction::copy(this, &StructureView::copyToClipboard,  this);
    m_copyAction->setShortcut(QKeySequence());

    m_copyOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                     i18nc("@action", "Copy Offset"), this);
    m_copyOffsetAction->setToolTip(i18nc("@info:tooltip",
                                         "Copies the offset to the clipboard."));
    connect(m_copyOffsetAction, &QAction::triggered,
            this, &StructureView::copyOffsetToClipboard);

    m_selectAction = new QAction(QIcon::fromTheme(QStringLiteral("select-rectangular")),
                                 i18nc("@action:inmenu", "Select in View"), this);
    connect(m_selectAction, &QAction::triggered,
            this, &StructureView::selectBytesInView);

    // UI content
    auto* const baseLayout = new QVBoxLayout(this);
    setLayout(baseLayout);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // table
    mStructureTreeModel = new StructureTreeModel(mTool, this);
    connect(mStructureTreeModel, &QAbstractItemModel::dataChanged,
            this, &StructureView::updateDataActions);
    // mModeltester = new QAbstractItemModelTester(mStructureTreeModel, this);
    mStructTreeView = new QTreeView(this);
    mStructTreeView->setObjectName(QStringLiteral("StructTree"));
    mStructTreeView->setAlternatingRowColors(true);
    mStructTreeView->setUniformRowHeights(true);
    mStructTreeView->setAllColumnsShowFocus(true);
    mStructTreeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    mStructTreeView->setItemDelegate(mDelegate);
    mStructTreeView->setDragEnabled(true);
    mStructTreeView->setDragDropMode(QAbstractItemView::DragOnly);
    mStructTreeView->setModel(mStructureTreeModel);
    mStructTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    mStructTreeView->installEventFilter(this);
    QHeaderView* const header = mStructTreeView->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    connect(mStructTreeView, &QWidget::customContextMenuRequested,
            this, &StructureView::onCustomContextMenuRequested);
    connect(mStructTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &StructureView::onDataSelectionChanged);

    // TODO. share code for all these empty-list placeholders
    QWidget* const structListViewViewPort = mStructTreeView->viewport();
    m_emptyListOverlayLabel = new QLabel(structListViewViewPort);
    m_emptyListOverlayLabel->setText(i18nc("@info", "No structure definitions"));
    m_emptyListOverlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_emptyListOverlayLabel->setEnabled(false);
    m_emptyListOverlayLabel->setWordWrap(true);
    m_emptyListOverlayLabel->setAlignment(Qt::AlignCenter);
    m_emptyListOverlayLabel->setVisible(mTool->isStructureListEmpty());
    auto* const centeringLayout = new QVBoxLayout(structListViewViewPort);
    centeringLayout->addWidget(m_emptyListOverlayLabel);
    centeringLayout->setAlignment(m_emptyListOverlayLabel, Qt::AlignCenter);
    connect(mTool, &StructuresTool::isStructureListEmptyChanged,
            m_emptyListOverlayLabel, &QWidget::setVisible);

    baseLayout->addWidget(mStructTreeView, 10);

    // settings
    auto* const actionsToolBar = new QToolBar(this);
    actionsToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    actionsToolBar->addAction(m_copyAction);

    auto* const stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(stretcher);

    actionsToolBar->addAction(mValidateAction);

    actionsToolBar->addAction(mLockStructureAction);

    auto* const menu = new KActionMenu(QIcon::fromTheme(QStringLiteral("overflow-menu")), QString(), this);
    menu->setPopupMode(QToolButton::InstantPopup);
    menu->addAction(m_editAction);
    menu->addSeparator();
    menu->addAction(m_copyOffsetAction);
    menu->addSeparator();
    menu->addAction(m_selectAction);
    menu->addSeparator();
    menu->addAction(mScriptConsoleAction);
    menu->addAction(mSettingsAction);
    actionsToolBar->addAction(menu);

    baseLayout->addWidget(actionsToolBar);

    connect(mTool, &StructuresTool::byteArrayModelChanged,
            this, &StructureView::onByteArrayModelChanged);

    // init state
    onDataSelectionChanged();
}

StructureView::~StructureView() = default;

StructuresTool* StructureView::tool() const
{
    return mTool;
}

void StructureView::openSettingsDlg()
{
    // An instance of your dialog could be already created and could be cached,
    // in which case you want to display the cached dialog instead of creating
    // another one
    if (KConfigDialog::showDialog(QStringLiteral("Structures Tool Settings"))) {
        return;
    }

    // KConfigDialog didn't find an instance of this dialog, so lets create it :
    auto* const dialog = new KConfigDialog(this, QStringLiteral("Structures Tool Settings"),
                                           StructureViewPreferences::self());

    auto* const displaySettings = new StructureViewSettingsWidget();
    KPageWidgetItem* const displ = dialog->addPage(displaySettings, i18nc("@title:tab", "Value Display"),
                                                   QStringLiteral("configure"));

    // cannot use StructuresManagerView directly as page even if the only element
    // because KConfigDialogManager only scans the children of the page for kcfg_ elements
    auto* const structSelectionPage = new QWidget();
    auto* const hbox = new QHBoxLayout();
    hbox->setContentsMargins(0, 0, 0, 0);
    structSelectionPage->setLayout(hbox);
    auto* const structureSettings = new StructuresManagerView(mTool, this);
    structureSettings->setObjectName(QStringLiteral("kcfg_LoadedStructures"));
    hbox->addWidget(structureSettings);
    dialog->addPage(structSelectionPage, i18nc("@title:tab", "Structures management"),
                    QStringLiteral("preferences-plugin"));

    // User edited the configuration - update your local copies of the configuration data
    connect(dialog, &KConfigDialog::settingsChanged, mTool, &StructuresTool::setEnabledStructuresInView);

    // TODO: kconfig_compiler signals work now, use those signals and not the generic KConfigDialog::settingsChanged
    dialog->setCurrentPage(displ);
    dialog->show();
}

bool StructureView::eventFilter(QObject* object, QEvent* event)
{
    if (object == mStructTreeView) {
        if (event->type() == QEvent::FocusIn) {
            const QModelIndex current = mStructTreeView->selectionModel()->currentIndex();

            if (current.isValid()) {
                mTool->mark(current);
            } else {
                mTool->unmark();
            }
        } else if (event->type() == QEvent::FocusOut) {
            QWidget* const treeViewFocusWidget = mStructTreeView->focusWidget();
            const bool subChildHasFocus = (treeViewFocusWidget != mStructTreeView);
            if (subChildHasFocus) {
                mStructTreeViewFocusChild = treeViewFocusWidget;
                mStructTreeViewFocusChild->installEventFilter(this);
            } else {
                mTool->unmark();
            }
        }
    } else if (object == mStructTreeViewFocusChild) {
        // TODO: it is only assumed the edit widget will be removed if it loses the focus
        if (event->type() == QEvent::FocusOut) {
            if (!mStructTreeView->hasFocus()) {
                mTool->unmark();
            }
            mStructTreeViewFocusChild->removeEventFilter(this);
            mStructTreeViewFocusChild = nullptr;
        }
    }

    return QWidget::eventFilter(object, event);
}

void StructureView::setLockButtonState(const QModelIndex& current)
{
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "setLockButtonState() for" << current;

    mLockStructureAction->setEnabled(mTool->canStructureBeLocked(current));
    mLockStructureAction->setChecked(mTool->isStructureLocked(current));
}

void StructureView::updateDataActions()
{
    const QItemSelectionModel* const selectionModel = mStructTreeView->selectionModel();

    const QModelIndexList selectedRows = selectionModel->selectedRows();
    const QModelIndex currentIndex = selectionModel->currentIndex();
    const auto* const data = currentIndex.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    const bool dataSelected =
        (data != nullptr) &&
        (selectedRows.size() == 1) &&
        selectionModel->isSelected(currentIndex);

    const bool hasData = dataSelected && data->wasAbleToRead();

    const bool isEditable = hasData && (currentIndex.siblingAtColumn(DataInformation::ColumnValue).flags() & Qt::ItemIsEditable);
    m_editAction->setEnabled(isEditable);

    m_copyAction->setEnabled(hasData);
    m_copyOffsetAction->setEnabled(hasData);
    m_selectAction->setEnabled(hasData);
}

void StructureView::onDataSelectionChanged()
{
    const QItemSelectionModel* const selectionModel = mStructTreeView->selectionModel();

    const QModelIndexList selectedRows = selectionModel->selectedRows();
    const QModelIndex currentIndex = selectionModel->currentIndex();
    const auto* const data = currentIndex.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    const bool dataSelected =
        (data != nullptr) &&
        (selectedRows.size() == 1) &&
        selectionModel->isSelected(currentIndex);

    if (dataSelected && mTool->byteArrayModel()) {
        mTool->mark(currentIndex);
    } else {
        mTool->unmark();
    }
    setLockButtonState(currentIndex);

    updateDataActions();
}

void StructureView::onLockButtonClicked(bool checked)
{
    // qCDebug(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "Lock button toggled";

    const QModelIndex current = mStructTreeView->selectionModel()->currentIndex();
    if (!current.isValid()) {
        qCWarning(LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES) << "it should not be possible to toggle this button when current index is invalid!";
        return;
    }

    if (checked) {
        mTool->lockStructure(current);
    } else {
        mTool->unlockStructure(current);
    }
}

void StructureView::onLockButtonToggled(bool structureLocked)
{
    if (structureLocked) {
        mLockStructureAction->setIcon(QIcon::fromTheme(QStringLiteral("object-locked")));
        mLockStructureAction->setText(i18nc("@action:button"
                                            " unlock the starting offset of the current structure", "Unlock"));
        mLockStructureAction->setToolTip(i18nc("@info:tooltip",
                                               "Unlock selected structure, i.e. the starting offset is"
                                               " always set to the current cursor position."));
    } else {
        mLockStructureAction->setIcon(QIcon::fromTheme(QStringLiteral("object-unlocked")));
        mLockStructureAction->setText(i18nc("@action:button"
                                            " unlock the starting offset of the current structure", "Lock"));
        mLockStructureAction->setToolTip(i18nc("@info:tooltip",
                                               "Lock selected structure to current offset."));
    }
}

void StructureView::editData()
{
    const QModelIndex index = mStructTreeView->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }

    mStructTreeView->setCurrentIndex(index);
    mStructTreeView->edit(index);
}

void StructureView::selectBytesInView()
{
    const QModelIndex index = mStructTreeView->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }

    mTool->selectBytesInView(index);
}

void StructureView::copyToClipboard()
{
    const QModelIndex index = mStructTreeView->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }

    QMimeData* const mimeData = mStructTreeView->model()->mimeData({index});

    QApplication::clipboard()->setMimeData(mimeData);
}

void StructureView::copyOffsetToClipboard()
{
    const QModelIndex index = mStructTreeView->selectionModel()->currentIndex();
    if (!index.isValid()) {
        return;
    }

    const auto* const data = index.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }

    const QString offsetText = mTool->dataAddressAsString(data);
    QApplication::clipboard()->setText(offsetText);
}

void StructureView::openScriptConsole()
{
    auto* const dialog = new QDialog(this);
    dialog->setWindowTitle(i18nc("@title:window", "Structures Script Console"));
    auto* const layout = new QVBoxLayout;
    auto* const dialogButtonBox = new QDialogButtonBox;
    QPushButton* const closeButton = dialogButtonBox->addButton(QDialogButtonBox::Close);
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(new ScriptLoggerView(mTool->allData()));
    layout->addWidget(dialogButtonBox);
    dialog->setLayout(layout);
    dialog->show();
}

void StructureView::onByteArrayModelChanged(Okteta::AbstractByteArrayModel* model)
{
    const bool validModel = (model != nullptr);
    const QModelIndex current = mStructTreeView->currentIndex();
    setLockButtonState(current);
    mValidateAction->setEnabled(validModel);
}

void StructureView::onCustomContextMenuRequested(QPoint pos)
{
    const QModelIndex index = mStructTreeView->indexAt(pos);
    if (!index.isValid()) {
        return;
    }
    const auto* const data = index.data(StructureTreeModel::DataInformationRole).value<DataInformation*>();
    if (!data) {
        return;
    }

    const bool isStructureRoot = data->parent()->isTopLevel();

    if (!isStructureRoot && !data->wasAbleToRead()) {
        return;
    }

    auto* const menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    if (data->wasAbleToRead()) {
        const QModelIndex valueIndex = index.siblingAtColumn(DataInformation::ColumnValue);
        if (valueIndex.flags() & Qt::ItemIsEditable) {
            menu->addAction(m_editAction);
        }

        menu->addAction(m_copyAction);

        menu->addAction(m_copyOffsetAction);

        menu->addAction(m_selectAction);
    }

    if (isStructureRoot) {
        if (!menu->isEmpty()) {
            menu->addSeparator();
        }
        menu->addAction(mLockStructureAction);
    }

    menu->popup(mStructTreeView->viewport()->mapToGlobal(pos));
}

}

#include "moc_structureview.cpp"

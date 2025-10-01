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
    auto* const baseLayout = new QVBoxLayout(this);
    setLayout(baseLayout);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // table
    mStructureTreeModel = new StructureTreeModel(mTool, this);
    // mModeltester = new QAbstractItemModelTester(mStructureTreeModel, this);
    mStructTreeView = new QTreeView(this);
    mStructTreeView->setObjectName(QStringLiteral("StructTree"));
    mStructTreeView->setRootIsDecorated(true);
    mStructTreeView->setAlternatingRowColors(true);
    mStructTreeView->setItemsExpandable(true);
    mStructTreeView->setUniformRowHeights(true);
    mStructTreeView->setAllColumnsShowFocus(true);
    mStructTreeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    mStructTreeView->setItemDelegate(mDelegate);
    mStructTreeView->setDragEnabled(true);
    mStructTreeView->setDragDropMode(QAbstractItemView::DragOnly);
    mStructTreeView->setSortingEnabled(false);
    mStructTreeView->setModel(mStructureTreeModel);
    mStructTreeView->setHeaderHidden(false);
    mStructTreeView->setSortingEnabled(false);
    mStructTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    mStructTreeView->installEventFilter(this);
    QHeaderView* const header = mStructTreeView->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    connect(mStructTreeView, &QWidget::customContextMenuRequested,
            this, &StructureView::onCustomContextMenuRequested);

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

    QIcon validateIcon = QIcon::fromTheme(QStringLiteral("document-sign"));
    mValidateAction =
        actionsToolBar->addAction(validateIcon, i18nc("@action:button", "Validate"),
                                  mTool, &StructuresTool::validateAllStructures);
    const QString validationToolTip = i18nc("@info:tooltip", "Validate all structures.");
    mValidateAction->setToolTip(validationToolTip);
    mValidateAction->setEnabled(false); // no point validating without file open
    connect(mTool, &StructuresTool::byteArrayModelChanged,
            this, &StructureView::onByteArrayModelChanged);
    // TODO also disable the button if the structure has no validatable members

    mLockStructureAction =
        actionsToolBar->addAction(QString(),
                                  this, &StructureView::onLockButtonClicked);
    mLockStructureAction->setCheckable(true);
    mLockStructureAction->setChecked(false);
    mLockStructureAction->setEnabled(false); // won't work at beginning
    onLockButtonToggled(false);
    connect(mLockStructureAction, &QAction::toggled, this, &StructureView::onLockButtonToggled);

    auto* const stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(stretcher);

    QIcon console = QIcon::fromTheme(QStringLiteral("utilities-terminal"));
    mScriptConsoleAction =
        actionsToolBar->addAction(console, i18nc("@action:button", "Script console"),
                                  this, &StructureView::openScriptConsole);
    mScriptConsoleAction->setToolTip(i18nc("@info:tooltip", "Open script console."));

    QIcon settings = QIcon::fromTheme(QStringLiteral("configure"));
    mSettingsAction =
        actionsToolBar->addAction(settings, i18nc("@action:button", "Settings"),
                                  this, &StructureView::openSettingsDlg);
    const QString settingsTooltip = i18nc("@info:tooltip", "Open settings.");
    mSettingsAction->setToolTip(settingsTooltip);

    baseLayout->addWidget(actionsToolBar);

    connect(mStructTreeView->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this, &StructureView::onCurrentRowChanged);
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

void StructureView::onCurrentRowChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(previous)
    if (current.isValid() && mTool->byteArrayModel()) {
        mTool->mark(current);
    } else {
        mTool->unmark();
    }
    setLockButtonState(current);
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
    auto* const action = static_cast<QAction*>(sender());
    const QModelIndex index = action->data().toModelIndex();

    mStructTreeView->setCurrentIndex(index);
    mStructTreeView->edit(index);
}

void StructureView::selectBytesInView()
{
    auto* const action = static_cast<QAction*>(sender());
    const QModelIndex index = action->data().toModelIndex();

    mTool->selectBytesInView(index);
}

void StructureView::copyToClipboard()
{
    auto* const action = static_cast<QAction*>(sender());
    const QModelIndex index = action->data().toModelIndex();
    QMimeData* const mimeData = mStructTreeView->model()->mimeData({index});

    QApplication::clipboard()->setMimeData(mimeData);
}

void StructureView::copyOffsetToClipboard()
{
    auto* const action = static_cast<QAction*>(sender());
    const QModelIndex index = action->data().toModelIndex();

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
            auto* const editAction = new QAction(QIcon::fromTheme(QStringLiteral("document-edit")),
                                                 i18nc("@action:inmenu", "Edit"), menu);
            connect(editAction, &QAction::triggered,
                    this, &StructureView::editData);
            editAction->setData(valueIndex);
            menu->addAction(editAction);
        }

        // TODO: split into explicit "Copy As Data" and "Copy As Text"
        QAction* const copyAction = KStandardAction::copy(this, &StructureView::copyToClipboard,  menu);
        copyAction->setShortcut(QKeySequence());
        copyAction->setData(index);
        menu->addAction(copyAction);

        auto* const copyOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                                   i18nc("@action", "Copy Offset"), this);
        copyOffsetAction->setToolTip(i18nc("@info:tooltip",
                                            "Copies the offset to the clipboard."));
        connect(copyOffsetAction, &QAction::triggered,
                this, &StructureView::copyOffsetToClipboard);
        copyOffsetAction->setData(index);
        menu->addAction(copyOffsetAction);

        auto* const selectAction = new QAction(QIcon::fromTheme(QStringLiteral("select-rectangular")),
                                               i18nc("@action:inmenu", "Select"), menu);
        connect(selectAction, &QAction::triggered,
                this, &StructureView::selectBytesInView);
        selectAction->setData(index);
        menu->addAction(selectAction);
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

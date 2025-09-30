/*
    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextractview.hpp"

// tool
#include "containedstringtablemodel.hpp"
#include "stringsextracttool.hpp"
// utils
#include <labelledtoolbarwidget.hpp>
// Kasten core
#include <Kasten/AbstractUserMessagesHandler>
#include <Kasten/UserNotification>
// KF
#include <KLocalizedString>
// Qt
#include <QMenu>
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QTreeView>
#include <QClipboard>
#include <QApplication>
#include <QFocusEvent>
#include <QAction>
#include <QIcon>
// Std
#include <utility>

namespace Kasten {

static constexpr int MinimumStringLength = 1;

StringsExtractView::StringsExtractView(StringsExtractTool* tool,
                                       AbstractUserMessagesHandler* userMessagseHandler,
                                       QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
    , m_userMessagesHandler(userMessagseHandler)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);
    baseLayout->setSpacing(0);

    // update
    auto* updateToolBar = new QToolBar(this);
    updateToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    auto* stretcher = new QWidget(this);
    stretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    updateToolBar->addWidget(stretcher);

    auto* label = new QLabel(i18nc("@label:spinbox minimum length for consecutive chars to be seen as a string",
                                   "Minimum length:"), this);

    mMinLengthSpinBox = new QSpinBox(this);
    mMinLengthSpinBox->setValue(mTool->minLength());
    mMinLengthSpinBox->setMinimum(MinimumStringLength);
    connect(mMinLengthSpinBox, qOverload<int>(&QSpinBox::valueChanged),
            mTool, &StringsExtractTool::setMinLength);
    auto* labelledMinLengthSpinBox = new LabelledToolBarWidget(label, mMinLengthSpinBox, this);
    updateToolBar->addWidget(labelledMinLengthSpinBox);

    mUpdateAction =
        updateToolBar->addAction(QIcon::fromTheme(QStringLiteral("document-export")),
                                 i18nc("@action:button extract the strings from the byte array", "&Extract"),
                                 mTool, &StringsExtractTool::extractStrings);
    mUpdateAction->setToolTip(i18nc("@info:tooltip",
                                    "Finds the strings contained in the selected range and lists them in the view below."));
    mUpdateAction->setWhatsThis(xi18nc("@info:whatsthis",
                                        "If you press the <interface>Extract</interface> button, "
                                        "the selected range is searched for all strings which have the set minimum length. "
                                        "This strings found will be listed in the view below."));
    mUpdateAction->setPriority(QAction::HighPriority);
    mUpdateAction->setEnabled(mTool->isApplyable());

    baseLayout->addWidget(updateToolBar);

    // filter
    auto* filterToolBar = new QToolBar(this);

    label = new QLabel(i18nc("@label:lineedit filter term for displayed strings", "Filter:"), this);

    auto* mFilterEdit = new QLineEdit(this);
    mFilterEdit->setClearButtonEnabled(true);
    mFilterEdit->setPlaceholderText(i18nc("@info:placeholder", "Enter a term to limit the list."));
    mFilterEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    auto* labelledFilterEdit = new LabelledToolBarWidget(label, mFilterEdit, this);
    filterToolBar->addWidget(labelledFilterEdit);

    baseLayout->addWidget(filterToolBar);

    // strings
    mContainedStringTableModel =
        new ContainedStringTableModel(mTool->containedStringList(), mTool->offsetCoding(), this);
    connect(mTool, &StringsExtractTool::offsetCodingChanged,
            mContainedStringTableModel, &ContainedStringTableModel::setOffsetCoding);

    mSortFilterProxyModel = new QSortFilterProxyModel(this);
    mSortFilterProxyModel->setDynamicSortFilter(true);
    mSortFilterProxyModel->setSourceModel(mContainedStringTableModel);
    mSortFilterProxyModel->setFilterKeyColumn(ContainedStringTableModel::StringColumnId);
    mSortFilterProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    connect(mFilterEdit, &QLineEdit::textChanged,
            mSortFilterProxyModel, &QSortFilterProxyModel::setFilterFixedString);

    mContainedStringTableView = new QTreeView(this);
    // TODO: find a signal/event emitted when fixedfont changes
//     connect( KGlobalSettings::self(), &KGlobalSettings::kdisplayFontChanged,
//              this, &StringsExtractView::setFixedFontByGlobalSettings );
    mContainedStringTableView->setObjectName(QStringLiteral("ContainedStringTable"));
    mContainedStringTableView->setRootIsDecorated(false);
    mContainedStringTableView->setItemsExpandable(false);
    mContainedStringTableView->setUniformRowHeights(true);
    mContainedStringTableView->setAllColumnsShowFocus(true);
    mContainedStringTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mContainedStringTableView->setDragEnabled(true);
    mContainedStringTableView->setDragDropMode(QAbstractItemView::DragOnly);
    mContainedStringTableView->setSortingEnabled(true);
    mContainedStringTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    mContainedStringTableView->installEventFilter(this);
    QHeaderView* header = mContainedStringTableView->header();
    header->setSectionResizeMode(QHeaderView::Interactive);
    mContainedStringTableView->setModel(mSortFilterProxyModel);
    mContainedStringTableView->sortByColumn(ContainedStringTableModel::OffsetColumnId, Qt::AscendingOrder);
    connect(mContainedStringTableView, &QTreeView::doubleClicked,
            this, &StringsExtractView::onStringDoubleClicked);
    connect(mContainedStringTableView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this, &StringsExtractView::onStringSelectionChanged);
    connect(mContainedStringTableView, &QWidget::customContextMenuRequested,
            this, &StringsExtractView::onCustomContextMenuRequested);

    // TODO. share code for all these empty-list placeholders
    auto* stringTableViewViewPort = mContainedStringTableView->viewport();
    m_emptyListOverlayLabel = new QLabel(stringTableViewViewPort);
    m_emptyListOverlayLabel->setText(i18nc("@info", "No strings"));
    m_emptyListOverlayLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_emptyListOverlayLabel->setEnabled(false);
    m_emptyListOverlayLabel->setWordWrap(true);
    m_emptyListOverlayLabel->setAlignment(Qt::AlignCenter);
    m_emptyListOverlayLabel->setVisible(mTool->containedStringList()->isEmpty());
    m_emptyListOverlayLabel->setContextMenuPolicy(Qt::NoContextMenu);
    auto* centeringLayout = new QVBoxLayout(stringTableViewViewPort);
    centeringLayout->addWidget(m_emptyListOverlayLabel);
    centeringLayout->setAlignment(m_emptyListOverlayLabel, Qt::AlignCenter);

    baseLayout->addWidget(mContainedStringTableView, 10);

    // actions
    auto* actionsToolBar = new QToolBar(this);
    actionsToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    mCopyAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                              i18nc("@action:button", "Copy"), this);
    mCopyAction->setToolTip(i18nc("@info:tooltip",
                                  "Copies the selected strings to the clipboard."));
    mCopyAction->setWhatsThis(xi18nc("@info:whatsthis",
                                     "If you press the <interface>Copy</interface> button, all strings you selected "
                                     "in the list are copied to the clipboard."));
    connect(mCopyAction, &QAction::triggered,
            this, &StringsExtractView::onCopyButtonClicked);
    actionsToolBar->addAction(mCopyAction);

    auto* actionsStretcher = new QWidget(this);
    actionsStretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(actionsStretcher);

    mGotoAction = new QAction(QIcon::fromTheme(QStringLiteral("go-jump")),
                              i18nc("@action:button", "Show"), this);
    mGotoAction->setToolTip(i18nc("@info:tooltip",
                                  "Shows the selected string in the view."));
    mGotoAction->setWhatsThis(xi18nc("@info:whatsthis",
                                     "If you press the <interface>Go to</interface> button, the string which was last "
                                     "selected is marked and shown in the view."));
    connect(mGotoAction, &QAction::triggered,
            this, &StringsExtractView::onGotoButtonClicked);
    actionsToolBar->addAction(mGotoAction);

    baseLayout->addWidget(actionsToolBar);

    mCopyOffsetAction = new QAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                    i18nc("@action", "Copy Offset"), this);
    mCopyOffsetAction->setToolTip(i18nc("@info:tooltip",
                                        "Copies the offset to the clipboard."));
    connect(mCopyOffsetAction, &QAction::triggered,
            this, &StringsExtractView::onCopyOffsetTriggered);

    mSelectAction = new QAction(QIcon::fromTheme(QStringLiteral("select-rectangular")),
                                i18nc("@action", "Select"), this);
    mSelectAction->setToolTip(i18nc("@info:tooltip",
                                    "Selects the selected string in the view."));
    connect(mSelectAction, &QAction::triggered,
            this, &StringsExtractView::onSelectStringTriggered);
    connect(mTool, &StringsExtractTool::uptodateChanged, this, &StringsExtractView::onStringsUptodateChanged);
    connect(mTool, &StringsExtractTool::isApplyableChanged, this, &StringsExtractView::onApplyableChanged);
    connect(mTool, &StringsExtractTool::extractionDone,
            this, &StringsExtractView::onExtractionDone);
    connect(mTool, &StringsExtractTool::canHighlightStringChanged, this, &StringsExtractView::onCanHighlightStringChanged);

    onStringSelectionChanged();
}

StringsExtractView::~StringsExtractView() = default;

bool StringsExtractView::eventFilter(QObject* object, QEvent* event)
{
    if (object == mContainedStringTableView) {
        if (event->type() == QEvent::FocusOut) {
            auto* const focusEvent = static_cast<QFocusEvent*>(event);
            const Qt::FocusReason focusReason = focusEvent->reason();
            if (focusReason != Qt::ActiveWindowFocusReason
                && focusReason != Qt::PopupFocusReason) {
                mTool->unmarkString();
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void StringsExtractView::onCustomContextMenuRequested(QPoint pos)
{
    const QModelIndex index = mContainedStringTableView->indexAt(pos);

    if (!index.isValid()) {
        return;
    }

    auto* menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);

    menu->addAction(mGotoAction);
    menu->addSeparator();
    menu->addAction(mCopyAction);
    menu->addAction(mCopyOffsetAction);
    menu->addSeparator();
    menu->addAction(mSelectAction);

    menu->popup(mContainedStringTableView->viewport()->mapToGlobal(pos));
}

void StringsExtractView::onStringsUptodateChanged(bool stringsUptodate)
{
    if (stringsUptodate) {
        mContainedStringTableModel->update();
    }

    const bool isApplyable = mTool->isApplyable();
    mUpdateAction->setEnabled(!stringsUptodate && isApplyable);
}

void StringsExtractView::onApplyableChanged(bool isApplyable)
{
    mUpdateAction->setEnabled(!mTool->isUptodate() && isApplyable);
}

void StringsExtractView::onCanHighlightStringChanged(bool canHighlightString)
{
    const bool stringSelected = mContainedStringTableView->selectionModel()->currentIndex().isValid();
    const bool isStringToMarkOrSelect = (canHighlightString && stringSelected);
    mGotoAction->setEnabled(isStringToMarkOrSelect);
    mSelectAction->setEnabled(isStringToMarkOrSelect);
}

void StringsExtractView::onGotoButtonClicked()
{
    const QModelIndex index = mContainedStringTableView->selectionModel()->currentIndex();
    if (index.isValid()) {
        // TODO: hack: as currently the marking is only undone if the focus leaves the listview it needs to be moved there before
        mContainedStringTableView->setFocus();
        onStringDoubleClicked(index);
    }
}

void StringsExtractView::onCopyButtonClicked()
{
    const QItemSelectionModel* selectionModel = mContainedStringTableView->selectionModel();

    const QModelIndexList selectedIndexes = selectionModel->selectedIndexes();
    QMimeData* mimeData = mContainedStringTableView->model()->mimeData(selectedIndexes);

    QApplication::clipboard()->setMimeData(mimeData);
}

void StringsExtractView::onCopyOffsetTriggered()
{
    const QModelIndex index = mContainedStringTableView->selectionModel()->currentIndex();
    if (index.isValid()) {
        const QString offsetText = index.data(ContainedStringTableModel::OffsetStringRole).toString();
        QApplication::clipboard()->setText(offsetText);
    }
}

void StringsExtractView::onSelectStringTriggered()
{
    const QModelIndex index = mContainedStringTableView->selectionModel()->currentIndex();
    if (index.isValid()) {
        if (mTool->canHighlightString()) {
            mTool->selectString(mSortFilterProxyModel->mapToSource(index).row());
        }
    }
}

void StringsExtractView::onStringSelectionChanged()
{
    const QItemSelectionModel* selectionModel = mContainedStringTableView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const bool hasSelection = !selectionModel->selectedIndexes().isEmpty();
    mCopyAction->setEnabled(hasSelection);

    const bool stringSelected = selectionModel->isSelected(selectionModel->currentIndex());
    const bool canHighlightString = mTool->canHighlightString();
    const bool isStringToMarkOrSelect = (canHighlightString && stringSelected);
    mGotoAction->setEnabled(isStringToMarkOrSelect);
    mSelectAction->setEnabled(isStringToMarkOrSelect);
}

void StringsExtractView::onStringDoubleClicked(const QModelIndex& index)
{
    if (mTool->canHighlightString()) {
        mTool->markString(mSortFilterProxyModel->mapToSource(index).row());
    }
}

void StringsExtractView::onExtractionDone(int extractedStringSCount)
{
    const bool isEmptyList = (extractedStringSCount == 0);

    m_emptyListOverlayLabel->setVisible(isEmptyList);

    const QString messageBoxTitle = mTool->title();
    const QString extractionReport = isEmptyList ?
        i18nc("@info", "No strings extracted.") :
        i18ncp("@info", "1 string extracted.", "%1 strings extracted.", extractedStringSCount);
    auto message = std::make_unique<Kasten::UserNotification>(mTool->targetModel(), extractionReport,
                                                              messageBoxTitle);
    m_userMessagesHandler->postNotification(std::move(message));
}

}

#include "moc_stringsextractview.cpp"

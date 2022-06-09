/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextractview.hpp"

// tool
#include "containedstringtablemodel.hpp"
#include "stringsextracttool.hpp"
// utils
#include <labelledtoolbarwidget.hpp>
// KF
#include <KLocalizedString>
// Qt
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>
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

namespace Kasten {

static constexpr int MinimumStringLength = 1;

StringsExtractView::StringsExtractView(StringsExtractTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
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

    QLabel* label = new QLabel(i18nc("@label:spinbox minimum length for consecutive chars to be seen as a string",
                                     "Minimum length:"), this);

    mMinLengthSpinBox = new QSpinBox(this);
    mMinLengthSpinBox->setValue(mTool->minLength());
    mMinLengthSpinBox->setMinimum(MinimumStringLength);
    connect(mMinLengthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
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
    mUpdateAction->setEnabled(mTool->isApplyable());

    baseLayout->addWidget(updateToolBar);

    // filter
    auto* filterToolBar = new QToolBar(this);

    label = new QLabel(i18nc("@label:lineedit filter term for displayed strings", "Filter:"), this);

    auto* mFilterEdit = new QLineEdit(this);
    mFilterEdit->setClearButtonEnabled(true);
    mFilterEdit->setPlaceholderText(i18n("Enter a term to limit the list."));
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
    mContainedStringTableView->setSortingEnabled(true);
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

    baseLayout->addWidget(mContainedStringTableView, 10);

    // actions
    auto* actionsToolBar = new QToolBar(this);
    actionsToolBar->setToolButtonStyle(Qt::ToolButtonFollowStyle);

    mCopyAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("edit-copy")),
                                  i18n("C&opy"),
                                  this, &StringsExtractView::onCopyButtonClicked);
    mCopyAction->setToolTip(i18nc("@info:tooltip",
                                  "Copies the selected strings to the clipboard."));
    mCopyAction->setWhatsThis(xi18nc("@info:whatsthis",
                                     "If you press the <interface>Copy</interface> button, all strings you selected "
                                     "in the list are copied to the clipboard."));

    auto* actionsStretcher = new QWidget(this);
    actionsStretcher->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    actionsToolBar->addWidget(actionsStretcher);

    mGotoAction =
        actionsToolBar->addAction(QIcon::fromTheme(QStringLiteral("go-jump")),
                                  i18n("&Show"),
                                  this, &StringsExtractView::onGotoButtonClicked);
    mGotoAction->setToolTip(i18nc("@info:tooltip",
                                  "Shows the selected string in the view."));
    mGotoAction->setWhatsThis(xi18nc("@info:whatsthis",
                                     "If you press the <interface>Go to</interface> button, the string which was last "
                                     "selected is marked and shown in the view."));

    baseLayout->addWidget(actionsToolBar);

    connect(mTool, &StringsExtractTool::uptodateChanged, this, &StringsExtractView::onStringsUptodateChanged);
    connect(mTool, &StringsExtractTool::isApplyableChanged, this, &StringsExtractView::onApplyableChanged);
    connect(mTool, &StringsExtractTool::canHighlightStringChanged, this, &StringsExtractView::onCanHighlightStringChanged);

    onStringSelectionChanged();
}

StringsExtractView::~StringsExtractView() = default;

bool StringsExtractView::eventFilter(QObject* object, QEvent* event)
{
    if (object == mContainedStringTableView) {
        if (event->type() == QEvent::FocusOut) {
            auto* focusEvent = static_cast<QFocusEvent*>(event);
            const Qt::FocusReason focusReason = focusEvent->reason();
            if (focusReason != Qt::ActiveWindowFocusReason
                && focusReason != Qt::PopupFocusReason) {
                mTool->unmarkString();
            }
        }
    }

    return QWidget::eventFilter(object, event);
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
    mGotoAction->setEnabled(canHighlightString && stringSelected);
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
    const QModelIndexList selectedRows = mContainedStringTableView->selectionModel()->selectedRows();
    const QList<ContainedString>* containedStringList = mTool->containedStringList();

    QString strings;
    for (const QModelIndex& index : selectedRows) {
        const int i = mSortFilterProxyModel->mapToSource(index).row();
        strings += containedStringList->at(i).string();
        strings += QLatin1Char('\n'); // TODO: specific linefeed for platforms
    }

    QApplication::clipboard()->setText(strings);
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
    mGotoAction->setEnabled(canHighlightString && stringSelected);
}

void StringsExtractView::onStringDoubleClicked(const QModelIndex& index)
{
    if (mTool->canHighlightString()) {
        mTool->markString(mSortFilterProxyModel->mapToSource(index).row());
    }
}

}

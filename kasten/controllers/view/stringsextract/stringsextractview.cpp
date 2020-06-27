/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    SPDX-FileCopyrightText: 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "stringsextractview.hpp"

// tool
#include "containedstringtablemodel.hpp"
#include "stringsextracttool.hpp"
// KF
#include <KGuiItem>
#include <KLocalizedString>
// Qt
#include <QFontDatabase>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QHeaderView>
#include <QTreeView>
#include <QClipboard>
#include <QApplication>
#include <QFocusEvent>

namespace Kasten {

static constexpr int MinimumStringLength = 1;

StringsExtractView::StringsExtractView(StringsExtractTool* tool, QWidget* parent)
    : QWidget(parent)
    , mTool(tool)
{
    auto* baseLayout = new QVBoxLayout(this);
    baseLayout->setContentsMargins(0, 0, 0, 0);

    // update
    auto* updateLayout = new QHBoxLayout();

    updateLayout->addStretch();
    QLabel* label = new QLabel(i18nc("@label:spinbox minimum length for consecutive chars to be seen as a string",
                                     "Minimum length:"), this);
    updateLayout->addWidget(label);

    mMinLengthSpinBox = new QSpinBox(this);
    mMinLengthSpinBox->setValue(mTool->minLength());
    mMinLengthSpinBox->setMinimum(MinimumStringLength);
    connect(mMinLengthSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            mTool, &StringsExtractTool::setMinLength);
    label->setBuddy(mMinLengthSpinBox);
    updateLayout->addWidget(mMinLengthSpinBox);

    const KGuiItem updateGuiItem =
        KGuiItem(i18nc("@action:button extract the strings from the byte array", "&Extract"),
                 QStringLiteral("document-export"),
                 i18nc("@info:tooltip",
                       "Finds the strings contained in the selected range and lists them in the view below."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Extract</interface> button, "
                        "the selected range is searched for all strings which have the set minimum length. "
                        "This strings found will be listed in the view below."));
    mUpdateButton = new QPushButton(this);
    KGuiItem::assign(mUpdateButton, updateGuiItem);
    mUpdateButton->setEnabled(mTool->isApplyable());
    connect(mUpdateButton, &QPushButton::clicked, mTool, &StringsExtractTool::extractStrings);
    updateLayout->addWidget(mUpdateButton);

    baseLayout->addLayout(updateLayout);

    // filter
    auto* filterLayout = new QHBoxLayout();

    label = new QLabel(i18nc("@label:lineedit filter term for displayed strings", "Filter:"), this);
    filterLayout->addWidget(label);

    auto* mFilterEdit = new QLineEdit(this);
    mFilterEdit->setClearButtonEnabled(true);
    mFilterEdit->setPlaceholderText(i18n("Enter a term to limit the list."));
    label->setBuddy(mFilterEdit);
    filterLayout->addWidget(mFilterEdit, 10);

    baseLayout->addLayout(filterLayout);

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
    setFixedFontByGlobalSettings(); // do this before setting model
    mContainedStringTableView->setObjectName(QStringLiteral("ContainedStringTable"));
    mContainedStringTableView->setRootIsDecorated(false);
    mContainedStringTableView->setItemsExpandable(false);
    mContainedStringTableView->setUniformRowHeights(true);
    mContainedStringTableView->setAllColumnsShowFocus(true);
    mContainedStringTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mContainedStringTableView->setSortingEnabled(true);
    mContainedStringTableView->installEventFilter(this);
    QHeaderView* header = mContainedStringTableView->header();
    header->setFont(font());
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
    auto* actionsLayout = new QHBoxLayout();

    const KGuiItem copyGuiItem =
        KGuiItem(i18n("C&opy"),
                 QStringLiteral("edit-copy"),
                 i18nc("@info:tooltip",
                       "Copies the selected strings to the clipboard."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Copy</interface> button, all strings you selected "
                        "in the list are copied to the clipboard."));
    mCopyButton = new QPushButton(this);
    KGuiItem::assign(mCopyButton, copyGuiItem);
    connect(mCopyButton, &QPushButton::clicked, this, &StringsExtractView::onCopyButtonClicked);
    actionsLayout->addWidget(mCopyButton);

    actionsLayout->addStretch();

    const KGuiItem gotoGuiItem =
        KGuiItem(i18n("&Show"),
                 QStringLiteral("go-jump"),
                 i18nc("@info:tooltip",
                       "Shows the selected string in the view."),
                 xi18nc("@info:whatsthis",
                        "If you press the <interface>Go to</interface> button, the string which was last "
                        "selected is marked and shown in the view."));
    mGotoButton = new QPushButton(this);
    KGuiItem::assign(mGotoButton, gotoGuiItem);
    connect(mGotoButton, &QPushButton::clicked, this, &StringsExtractView::onGotoButtonClicked);
    actionsLayout->addWidget(mGotoButton);

    baseLayout->addLayout(actionsLayout);

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

void StringsExtractView::setFixedFontByGlobalSettings()
{
    mContainedStringTableView->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
}

void StringsExtractView::onStringsUptodateChanged(bool stringsUptodate)
{
    if (stringsUptodate) {
        mContainedStringTableModel->update();
    }

    const bool isApplyable = mTool->isApplyable();
    mUpdateButton->setEnabled(!stringsUptodate && isApplyable);
}

void StringsExtractView::onApplyableChanged(bool isApplyable)
{
    mUpdateButton->setEnabled(!mTool->isUptodate() && isApplyable);
}

void StringsExtractView::onCanHighlightStringChanged(bool canHighlightString)
{
    const bool stringSelected = mContainedStringTableView->selectionModel()->currentIndex().isValid();
    mGotoButton->setEnabled(canHighlightString && stringSelected);
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
    mCopyButton->setEnabled(hasSelection);

    const bool stringSelected = selectionModel->isSelected(selectionModel->currentIndex());
    const bool canHighlightString = mTool->canHighlightString();
    mGotoButton->setEnabled(canHighlightString && stringSelected);
}

void StringsExtractView::onStringDoubleClicked(const QModelIndex& index)
{
    if (mTool->canHighlightString()) {
        mTool->markString(mSortFilterProxyModel->mapToSource(index).row());
    }
}

}

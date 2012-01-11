/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2007-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library. If not, see <http://www.gnu.org/licenses/>.
*/

#include "stringsextractview.h"

// tool
#include "containedstringtablemodel.h"
#include "stringsextracttool.h"
// KDE
#include <KPushButton>
#include <KLineEdit>
#include <KGuiItem>
#include <KLocale>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>
#include <QtGui/QClipboard>
#include <QtGui/QApplication>
#include <QtGui/QFocusEvent>


namespace Kasten2
{

static const int MinimumStringLength = 1;

StringsExtractView::StringsExtractView( StringsExtractTool *tool, QWidget* parent )
 : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // update
    QHBoxLayout *updateLayout = new QHBoxLayout();

    updateLayout->addStretch();
    QLabel *label = new QLabel( i18nc("@label:spinbox minimum length for consecutive chars to be seen as a string",
                                      "Minimum length:"), this );
    updateLayout->addWidget( label );

    mMinLengthSpinBox = new QSpinBox( this );
    mMinLengthSpinBox->setValue( mTool->minLength() );
    mMinLengthSpinBox->setMinimum( MinimumStringLength );
    connect( mMinLengthSpinBox, SIGNAL(valueChanged(int)), mTool, SLOT(setMinLength(int)) );
    label->setBuddy( mMinLengthSpinBox );
    updateLayout->addWidget( mMinLengthSpinBox );

    const KGuiItem updateGuiItem =
        KGuiItem( i18nc("@action:button extract the strings from the byte array","&Extract"),
                  QLatin1String("document-export"),
                  i18nc("@info:tooltip",
                        "Finds the strings contained in the selected range and lists them in the view below."),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Extract</interface> button, "
                        "the selected range is searched for all strings which have the set minimum length. "
                        "This strings found will be listed in the view below.") );
    mUpdateButton = new KPushButton( updateGuiItem, this );
    mUpdateButton->setEnabled( mTool->isApplyable() );
    connect( mUpdateButton, SIGNAL(clicked(bool)), mTool, SLOT(extractStrings()) );
    updateLayout->addWidget( mUpdateButton );

    baseLayout->addLayout( updateLayout );

    // filter 
    QHBoxLayout *filterLayout = new QHBoxLayout();

    label = new QLabel( i18nc("@label:lineedit filter term for displayed strings","Filter:"), this );
    filterLayout->addWidget( label );

    KLineEdit *mFilterEdit = new KLineEdit( this );
    mFilterEdit->setClearButtonShown( true );
    mFilterEdit->setClickMessage( i18n("Enter a term to limit the list.") );
    label->setBuddy( mFilterEdit );
    filterLayout->addWidget( mFilterEdit, 10 );

    baseLayout->addLayout( filterLayout );

    // strings
    mContainedStringTableModel = new ContainedStringTableModel( mTool->containedStringList(), this );

    mSortFilterProxyModel = new QSortFilterProxyModel( this );
    mSortFilterProxyModel->setDynamicSortFilter( true );
    mSortFilterProxyModel->setSourceModel( mContainedStringTableModel );
    mSortFilterProxyModel->setFilterKeyColumn( ContainedStringTableModel::StringColumnId );
    mSortFilterProxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
    connect( mFilterEdit, SIGNAL(textChanged(QString)),
             mSortFilterProxyModel, SLOT(setFilterFixedString(QString)) );

    mContainedStringTableView = new QTreeView( this );
    connect( KGlobalSettings::self(), SIGNAL(kdisplayFontChanged()),
             SLOT(setFixedFontByGlobalSettings()) );
    setFixedFontByGlobalSettings(); //do this before setting model
    mContainedStringTableView->setObjectName( QLatin1String( "ContainedStringTable" ) );
    mContainedStringTableView->setRootIsDecorated( false );
    mContainedStringTableView->setItemsExpandable( false );
    mContainedStringTableView->setUniformRowHeights( true );
    mContainedStringTableView->setAllColumnsShowFocus( true );
    mContainedStringTableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    mContainedStringTableView->setSortingEnabled( true );
    mContainedStringTableView->installEventFilter( this );
    QHeaderView* header = mContainedStringTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::Interactive );
    mContainedStringTableView->setModel( mSortFilterProxyModel );
    mContainedStringTableView->sortByColumn( ContainedStringTableModel::OffsetColumnId, Qt::AscendingOrder );
    connect( mContainedStringTableView, SIGNAL(doubleClicked(QModelIndex)),
             SLOT(onStringDoubleClicked(QModelIndex)) );
    connect( mContainedStringTableView->selectionModel(),
             SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
             SLOT(onStringSelectionChanged()) );

    baseLayout->addWidget( mContainedStringTableView, 10 );

    // actions
    QHBoxLayout *actionsLayout = new QHBoxLayout();

    const KGuiItem copyGuiItem =
        KGuiItem( i18n("C&opy"),
                  QLatin1String("edit-copy"),
                  i18nc("@info:tooltip",
                        "Copies the selected strings to the clipboard."),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Copy</interface> button, all strings you selected "
                        "in the list are copied to the clipboard.") );
    mCopyButton = new KPushButton( copyGuiItem, this );
    connect( mCopyButton, SIGNAL(clicked(bool)), SLOT(onCopyButtonClicked()) );
    actionsLayout->addWidget( mCopyButton );

    actionsLayout->addStretch();

    const KGuiItem gotoGuiItem =
        KGuiItem( i18n("&Show"),
                  QLatin1String("go-jump"),
                  i18nc("@info:tooltip",
                        "Shows the selected string in the view."),
                  i18nc("@info:whatsthis",
                        "If you press the <interface>Go to</interface> button, the string which was last "
                        "selected is marked and shown in the view.") );
    mGotoButton = new KPushButton( gotoGuiItem, this );
    connect( mGotoButton, SIGNAL(clicked(bool)), SLOT(onGotoButtonClicked()) );
    actionsLayout->addWidget( mGotoButton );

    baseLayout->addLayout( actionsLayout );

    connect( mTool, SIGNAL(uptodateChanged(bool)), SLOT(onStringsUptodateChanged(bool)) );
    connect( mTool, SIGNAL(isApplyableChanged(bool)), SLOT(onApplyableChanged(bool)));
    connect( mTool, SIGNAL(canHighlightStringChanged(bool)), SLOT(onCanHighlightStringChanged(bool)) );

    onStringSelectionChanged();
}

bool StringsExtractView::eventFilter( QObject* object, QEvent* event )
{
    if( object == mContainedStringTableView )
    {
        if( event->type() == QEvent::FocusOut )
        {
            QFocusEvent* focusEvent = static_cast<QFocusEvent*>( event );
            const Qt::FocusReason focusReason = focusEvent->reason();
            if( focusReason != Qt::ActiveWindowFocusReason
                && focusReason != Qt::PopupFocusReason )
                mTool->unmarkString();
        }
    }

    return QWidget::eventFilter( object, event );
}

void StringsExtractView::setFixedFontByGlobalSettings()
{
    mContainedStringTableView->setFont( KGlobalSettings::fixedFont() );
}

void StringsExtractView::onStringsUptodateChanged( bool stringsUptodate )
{
    if( stringsUptodate )
        mContainedStringTableModel->update();

    const bool isApplyable = mTool->isApplyable();
    mUpdateButton->setEnabled( !stringsUptodate && isApplyable );
}

void StringsExtractView::onApplyableChanged( bool isApplyable )
{
    mUpdateButton->setEnabled( !mTool->isUptodate() && isApplyable );
}

void StringsExtractView::onCanHighlightStringChanged( bool canHighlightString )
{
    const bool stringSelected = mContainedStringTableView->selectionModel()->currentIndex().isValid();
    mGotoButton->setEnabled( canHighlightString && stringSelected );
}


void StringsExtractView::onGotoButtonClicked()
{
    const QModelIndex index = mContainedStringTableView->selectionModel()->currentIndex();
    if( index.isValid() )
    {
        // TODO: hack: as currently the marking is only undone if the focus leaves the listview it needs to be moved there before
        mContainedStringTableView->setFocus();
        onStringDoubleClicked( index );
    }
}

void StringsExtractView::onCopyButtonClicked()
{
    const QModelIndexList selectedRows = mContainedStringTableView->selectionModel()->selectedRows();
    const QList<ContainedString> *containedStringList = mTool->containedStringList();

    QString strings;
    foreach( const QModelIndex &index, selectedRows )
    {
        const int i = mSortFilterProxyModel->mapToSource(index).row();
        strings += containedStringList->at( i ).string();
        strings += QLatin1Char('\n'); //TODO: specific linefeed for platforms
    }
    QApplication::clipboard()->setText( strings );
}

void StringsExtractView::onStringSelectionChanged()
{
    const QItemSelectionModel *selectionModel = mContainedStringTableView->selectionModel();

    // TODO: selectionModel->selectedIndexes() is a expensive operation,
    // but with Qt 4.4.3 hasSelection() has the flaw to return true with a current index
    const bool hasSelection = !selectionModel->selectedIndexes().isEmpty();
    mCopyButton->setEnabled( hasSelection );

    const bool stringSelected = selectionModel->isSelected( selectionModel->currentIndex() );
    const bool canHighlightString = mTool->canHighlightString();
    mGotoButton->setEnabled( canHighlightString && stringSelected );
}

void StringsExtractView::onStringDoubleClicked( const QModelIndex &index )
{
    if( mTool->canHighlightString() )
        mTool->markString( mSortFilterProxyModel->mapToSource(index).row() );
}

StringsExtractView::~StringsExtractView() {}

}

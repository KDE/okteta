/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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


static const int MinimumStringLength = 1;

StringsExtractView::StringsExtractView( StringsExtractTool *tool, QWidget *parent )
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
    connect( mMinLengthSpinBox, SIGNAL(valueChanged( int )), mTool, SLOT(setMinLength( int )) );
    label->setBuddy( mMinLengthSpinBox );
    updateLayout->addWidget( mMinLengthSpinBox );

    const KGuiItem updateGuiItem( i18nc("@action:button update the list of strings extracted","&Update"), "view-refresh",
                      i18nc("@info:tooltip","Update the list of strings contained in the selected range."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Update</interface> button, the list will be updated "
                            "to all strings which are contained in the selected range and have the set minimum length.") );
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
    mFilterEdit->setClickMessage( i18n("Enter a filter term here.") );
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
    connect( mFilterEdit, SIGNAL(textChanged( const QString & )),
             mSortFilterProxyModel, SLOT(setFilterFixedString( const QString & )) );

    mContainedStringTableView = new QTreeView( this );
    mContainedStringTableView->setObjectName( "ContainedStringTable" );
    mContainedStringTableView->setRootIsDecorated( false );
    mContainedStringTableView->setItemsExpandable( false );
    mContainedStringTableView->setUniformRowHeights( true );
    mContainedStringTableView->setAllColumnsShowFocus( true );
    mContainedStringTableView->setSelectionMode( QAbstractItemView::ExtendedSelection );
    mContainedStringTableView->setSortingEnabled( true );
    mContainedStringTableView->setFont( KGlobalSettings::fixedFont() );
    QHeaderView* header = mContainedStringTableView->header();
    header->setFont( font() );
    header->setResizeMode( QHeaderView::ResizeToContents );
    mContainedStringTableView->setModel( mSortFilterProxyModel );
    mContainedStringTableView->sortByColumn( ContainedStringTableModel::OffsetColumnId, Qt::AscendingOrder );
    connect( mContainedStringTableView, SIGNAL(doubleClicked( const QModelIndex& )),
             SLOT(onStringDoubleClicked( const QModelIndex& )) );
    connect( mContainedStringTableView->selectionModel(),
             SIGNAL(selectionChanged( const QItemSelection &, const QItemSelection & )),
             SLOT(onStringSelectionChanged()) );

    baseLayout->addWidget( mContainedStringTableView, 10 );

    // actions
    QHBoxLayout *actionsLayout = new QHBoxLayout();

    const KGuiItem copyGuiItem( i18n("C&opy"), "edit-copy",
                      i18nc("@info:tooltip","Copies the selected strings to the clipboard."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Copy</interface> button, all strings you selected "
                            "in the list are copied to the clipboard.") );
    mCopyButton = new KPushButton( copyGuiItem, this );
    connect( mCopyButton, SIGNAL(clicked(bool)), SLOT(onCopyButtonClicked()) );
    actionsLayout->addWidget( mCopyButton );

    actionsLayout->addStretch();

    const KGuiItem gotoGuiItem( i18n("&Go to"), "go-jump",
                      i18nc("@info:tooltip","Shows the selected string in the view."),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Go to</interface> button, the string which was last "
                            "selected is marked and shown in the view.") );
    mGotoButton = new KPushButton( gotoGuiItem, this );
    connect( mGotoButton, SIGNAL(clicked(bool)), SLOT(onGotoButtonClicked()) );
    actionsLayout->addWidget( mGotoButton );

    baseLayout->addLayout( actionsLayout );

    connect( mTool, SIGNAL(uptodateChanged( bool )), SLOT(onStringsUptodateChanged( bool )) );
    connect( mTool, SIGNAL(isApplyableChanged( bool )), SLOT( onApplyableChanged( bool )) );
    connect( mTool, SIGNAL(canHighlightStringChanged( bool )), SLOT(onCanHighlightStringChanged( bool )) );

    onStringSelectionChanged();
}

#if 0
void StringsExtractView::setDirty( bool dirty )
{
    mUpdateButton->setToolTip( dirty ? i18nc("@info:tooltip","Warning: Byte Array has been modified since last update.") : QString() );
}
#endif

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
        onStringDoubleClicked( index );
}

void StringsExtractView::onCopyButtonClicked()
{
    const QModelIndexList selectedRows = mContainedStringTableView->selectionModel()->selectedRows();
    const QList<ContainedString> *containedStringList = mTool->containedStringList();

    QString strings;
    foreach( const QModelIndex &index, selectedRows )
    {
        const int i = mSortFilterProxyModel->mapToSource(index).row();
        strings += ( containedStringList->at( i ).string() + '\n' ); //TODO: specific linefeed for platforms
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
        mTool->selectString( mSortFilterProxyModel->mapToSource(index).row() );
}

StringsExtractView::~StringsExtractView() {}

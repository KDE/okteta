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
#include <KDialog>
#include <KLocale>
#include <KGlobalSettings>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QSpinBox>
#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QHeaderView>
#include <QtGui/QTreeView>


static const int MinimumStringLength = 1;

StringsExtractView::StringsExtractView( StringsExtractTool *tool, QWidget *parent )
 : QWidget( parent ), mTool( tool )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( KDialog::spacingHint() );

    // update
    QHBoxLayout *updateLayout = new QHBoxLayout();

    updateLayout->addStretch();
    QLabel *label = new QLabel( i18nc("@label:spinbox minimum length consecutive chars make a string","Minimum length:"), this );
    label->setFixedWidth( label->sizeHint().width() );
    updateLayout->addWidget( label );

    mMinLengthSpinBox = new QSpinBox( this );
    mMinLengthSpinBox->setValue( mTool->minLength() );
    mMinLengthSpinBox->setMinimum( MinimumStringLength );
    connect( mMinLengthSpinBox, SIGNAL(valueChanged( int )), mTool, SLOT(setMinLength( int )) );
    updateLayout->addWidget( mMinLengthSpinBox );

    mUpdateButton = new KPushButton( i18nc("@action:button update the list of strings extracted","Update"), this );
    mUpdateButton->setEnabled( mTool->isApplyable() );
    connect( mTool, SIGNAL(isApplyableChanged( bool )), mUpdateButton, SLOT( setEnabled(bool )) );
    connect( mUpdateButton, SIGNAL(clicked(bool)), SLOT(onExtractButtonClicked()) );
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
    connect( mTool, SIGNAL(stringsUpdated()), mContainedStringTableModel, SLOT(update()) );

    mSortFilterProxyModel = new QSortFilterProxyModel( this );
    mSortFilterProxyModel->setDynamicSortFilter( true );
    mSortFilterProxyModel->setSourceModel( mContainedStringTableModel );
    mSortFilterProxyModel->setFilterKeyColumn( ContainedStringTableModel::StringId );
    mSortFilterProxyModel->setFilterCaseSensitivity( Qt::CaseInsensitive );
    connect( mFilterEdit, SIGNAL(textChanged( const QString & )),
             mSortFilterProxyModel, SLOT(setFilterFixedString( const QString & )) );

    QTreeView *containedStringTableView = new QTreeView( this );
    containedStringTableView->setObjectName( "ContainedStringTable" );
    containedStringTableView->setRootIsDecorated( false );
    containedStringTableView->setItemsExpandable( false );
    containedStringTableView->setUniformRowHeights( true );
    containedStringTableView->setAllColumnsShowFocus( true );
    containedStringTableView->setSortingEnabled( true );
    containedStringTableView->setFont( KGlobalSettings::fixedFont() );
    containedStringTableView->header()->setFont( font() );
    containedStringTableView->setModel( mSortFilterProxyModel );
    containedStringTableView->sortByColumn( ContainedStringTableModel::OffsetId, Qt::AscendingOrder );
    connect( containedStringTableView, SIGNAL(clicked( const QModelIndex& )),
             SLOT(onStringClicked( const QModelIndex& )) );

    baseLayout->addWidget( containedStringTableView, 10 );
}

#if 0
void StringsExtractView::setDirty( bool dirty )
{
    mUpdateButton->setToolTip( dirty ? i18nc("@info:tooltip","Warning: Byte Array has been modified since last update.") : QString() );
}
#endif
void StringsExtractView::onExtractButtonClicked()
{
    mUpdateButton->setEnabled( false );
    mTool->extractStrings();
}

void StringsExtractView::onStringClicked( const QModelIndex &index )
{
// TODO: this works only if the view is the same and no changes happened
//     mTool->selectString( mSortFilterProxyModel->mapToSource(index).row() );
}



StringsExtractView::~StringsExtractView() {}

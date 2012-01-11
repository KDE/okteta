/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "podtableview.h"

// controller
#include "podtablemodel.h"
#include "poddelegate.h"
#include "poddecodertool.h"
// KDE
#include <KComboBox>
#include <KLocale>
#include <KMessageBox>
// Qt
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QCheckBox>
#include <QtGui/QTreeView>
#include <QtGui/QHeaderView>
#include <QtGui/QFocusEvent>
#include <QtGui/QFontMetrics>

#include <KDebug>
namespace Kasten2
{

PODTableView::PODTableView( PODDecoderTool* tool, QWidget* parent )
  : QWidget( parent ),
    mTool( tool ),
    mPODTableViewFocusChild( 0 )
{
    QBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // table
    mPODTableModel = new PODTableModel( mTool, this );
    mPODTableView = new QTreeView( this );
    mPODTableView->setObjectName( QLatin1String( "PODTable" ) );
    mPODTableView->setRootIsDecorated( false );
    mPODTableView->setAlternatingRowColors( true );
    mPODTableView->setItemsExpandable( false );
    mPODTableView->setUniformRowHeights( true );
    mPODTableView->setAllColumnsShowFocus( true );
    mPODTableView->setItemDelegate( new PODDelegate(mTool) );
    mPODTableView->setEditTriggers( QAbstractItemView::EditKeyPressed | QAbstractItemView::DoubleClicked );
    mPODTableView->setDragEnabled( true );
    mPODTableView->setSortingEnabled( false );
    mPODTableView->setModel( mPODTableModel );
    mPODTableView->installEventFilter( this );
    QHeaderView* header = mPODTableView->header();
    header->setResizeMode( QHeaderView::Interactive );
    header->setStretchLastSection( false );
    connect( mPODTableView->selectionModel(),
             SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
             SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)) );

    baseLayout->addWidget( mPODTableView, 10 );

    // settings
    QBoxLayout *settingsLayout = new QHBoxLayout();
    settingsLayout->setMargin( 0 );

    mByteOrderSelection = new KComboBox( this );
    mByteOrderSelection->addItem( i18nc("@item:inlistbox","Little-endian") ); // add first for index
    mByteOrderSelection->addItem( i18nc("@item:inlistbox","Big-endian") );    // add second for index
    mByteOrderSelection->setCurrentIndex( mTool->byteOrder() );
    connect( mByteOrderSelection, SIGNAL(activated(int)),
             mTool, SLOT(setByteOrder(int)));
    const QString byteOrderToolTip =
        i18nc( "@info:tooltip",
               "The byte order to use for decoding the bytes." );
    mByteOrderSelection->setToolTip( byteOrderToolTip );
    settingsLayout->addWidget( mByteOrderSelection );

    QLabel* unsignedAsHexLabel = new QLabel( i18nc("@option:check","Unsigned as hexadecimal:"), this );
    settingsLayout->addWidget( unsignedAsHexLabel );

    mUnsignedAsHexCheck = new QCheckBox( this );
    mUnsignedAsHexCheck->setChecked( mTool->isUnsignedAsHex() );
    connect( mUnsignedAsHexCheck, SIGNAL(toggled(bool)),
             mTool, SLOT(setUnsignedAsHex(bool)) );
    unsignedAsHexLabel->setBuddy( mUnsignedAsHexCheck );
    const QString unsignedAsHexToolTip =
        i18nc( "@info:tooltip",
               "Sets whether the values of the unsigned integer types are shown as hexadecimal instead of as decimal." );
    unsignedAsHexLabel->setToolTip( unsignedAsHexToolTip );
    mUnsignedAsHexCheck->setToolTip( unsignedAsHexToolTip );
    settingsLayout->addWidget( mUnsignedAsHexCheck );
    settingsLayout->addStretch();

    baseLayout->addLayout( settingsLayout );

    mTool->setDifferentSizeDialog( this );
    
    //resize to fit width of contents
    //this is much (!) faster than using setResizeMode(QHeaderView::ResizeToContents)
    QFont f;
    QFontMetrics metrics( f );
    //ideally we should check the width of the longest translated string, but this should be wide enough for most
    //anyway this is just an initial setting and the width can be changed manually
    header->resizeSection( 0, metrics.width( QLatin1String( "Hexadecimal 8-bit" ) ) + 30 );
    header->resizeSection( 1, metrics.width( QLatin1String( "1.01234567890123456789e-111" ) ) + 15 );
}

Answer PODTableView::query( int newValueSize, int oldValueSize, int sizeLeft )
{
    Q_UNUSED( sizeLeft );

    Answer answer;

    int messageBoxAnswer;
    if( newValueSize < oldValueSize )
    {
        const QString message =
            i18nc( "@info",
                    "The new value needs <emphasis>fewer</emphasis> bytes (%1 instead of %2).<nl/>"
                    "Keep the unused bytes or remove them?", newValueSize, oldValueSize );

        const KGuiItem keepGuiItem =
            KGuiItem( i18nc("@action:button keep the unused bytes",
                            "&Keep"),
                      QString(),
                      i18nc("@info:tooltip",
                            "Keep the unused bytes with their old values.") );

        messageBoxAnswer = KMessageBox::warningYesNoCancel( this, message, mTool->title(),
                                                            keepGuiItem,
                                                            KStandardGuiItem::remove() );
    }
    else
    {
        const QString message =
            i18nc( "@info",
                    "The new value needs <emphasis>more</emphasis> bytes (%1 instead of %2).<nl/>"
                    "Overwrite the following bytes or insert new ones as needed?", newValueSize, oldValueSize );

        messageBoxAnswer = KMessageBox::warningYesNoCancel( this, message, mTool->title(),
                                                            KStandardGuiItem::overwrite(),
                                                            KStandardGuiItem::insert() );
    }

    answer = (messageBoxAnswer == KMessageBox::Yes) ? Overwrite :
             (messageBoxAnswer == KMessageBox::No) ?  AdaptSize :
                                                      Cancel;
    return answer;
}

bool PODTableView::eventFilter( QObject* object, QEvent* event )
{
    if( object == mPODTableView )
    {
        if( event->type() == QEvent::FocusIn )
        {
            const QModelIndex current = mPODTableView->selectionModel()->currentIndex();
            const int podId = current.row();
            if( current.isValid() && mTool->isApplyable() && ! mTool->value(podId).isNull() )
                mTool->markPOD( podId );
        }
        else if( event->type() == QEvent::FocusOut )
        {
            QWidget* tableViewFocusWidget = mPODTableView->focusWidget();
            const bool subChildHasFocus = ( tableViewFocusWidget != mPODTableView );
            if( subChildHasFocus )
            {
                mPODTableViewFocusChild = tableViewFocusWidget;
                mPODTableViewFocusChild->installEventFilter( this );
            }
            else if( mTool->isApplyable() )
                mTool->unmarkPOD();
        }
    }
    else if( object == mPODTableViewFocusChild )
    {
        // TODO: it is only assumed the edit widget will be removed if it loses the focus
        if( event->type() == QEvent::FocusOut )
        {
            if( ! mPODTableView->hasFocus() && mTool->isApplyable() )
                mTool->unmarkPOD();
            mPODTableViewFocusChild->removeEventFilter( this );
            mPODTableViewFocusChild = 0;
        }
    }

    return QWidget::eventFilter( object, event );
}

void PODTableView::onCurrentRowChanged( const QModelIndex& current, const QModelIndex& previous )
{
    Q_UNUSED( previous )

    if( ! mTool->isApplyable() )
        return;

    const int podId = current.row();
    if( current.isValid() && ! mTool->value(podId).isNull() )
        mTool->markPOD( podId );
    else
        mTool->unmarkPOD();
}

PODTableView::~PODTableView() {}

}

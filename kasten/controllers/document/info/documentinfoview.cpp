/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "documentinfoview.h"

// 
#include "documentinfotool.h"
// KDE
#include <KLocale>
#include <KIconLoader>
#include <KSeparator>
#include <KSqueezedTextLabel>
#include <KGlobal>
#include <KDateTime>
#include <kio/netaccess.h>
// Qt
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QGridLayout>


namespace Kasten2
{

DocumentInfoView::DocumentInfoView( DocumentInfoTool* tool, QWidget* parent )
  : QWidget( parent ), mTool( tool )
{
    QVBoxLayout* baseLayout = new QVBoxLayout( this );
    baseLayout->setMargin( 0 );

    // icon
    mIconLabel = new QLabel( this );
//     int bsize = 66 + 2 * mIconLabel->style()->pixelMetric( QStyle::PM_ButtonMargin );
//     mIconLabel->setFixedSize(bsize, bsize);
	mIconLabel->setFixedHeight( KIconLoader::SizeEnormous );
	mIconLabel->setMinimumWidth( KIconLoader::SizeEnormous );
    mIconLabel->setAlignment( Qt::AlignHCenter );
    baseLayout->addWidget( mIconLabel );

    // file label
    mDocumentTitleLabel = new QLabel( this );
    QFont font = mDocumentTitleLabel->font();
    font.setBold(true);
    mDocumentTitleLabel->setFont( font );
    mDocumentTitleLabel->setAlignment( Qt::AlignHCenter );
    mDocumentTitleLabel->setWordWrap( true );
    mDocumentTitleLabel->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
    baseLayout->addWidget( mDocumentTitleLabel );

    // separator
    KSeparator* separator = new KSeparator( Qt::Horizontal, this );
    baseLayout->addWidget( separator );

    // property grid
    QGridLayout *propertyGrid = new QGridLayout(); // unknown rows
    propertyGrid->setColumnStretch( 0, 0 );
    propertyGrid->setColumnStretch( 1, 1 );

    int currentPropertyRow = 0;

    // type property
    QLabel* label = new QLabel( i18n("Type:"), this );
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight);

    mMimeTypeLabel = new QLabel( QString(), this );
    propertyGrid->addWidget( mMimeTypeLabel, currentPropertyRow++, 1);

    // location property
    label = new QLabel( i18n("Location:"), this );
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight);

    mLocationLabel = new KSqueezedTextLabel( this );
    // force the layout direction to be always LTR
    mLocationLabel->setLayoutDirection( Qt::LeftToRight );
    // but if we are in RTL mode, align the text to the right
    // otherwise the text is on the wrong side of the dialog
    if( layoutDirection() == Qt::RightToLeft )
	   mLocationLabel->setAlignment( Qt::AlignRight );
    mLocationLabel->setTextInteractionFlags( Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard );
    propertyGrid->addWidget( mLocationLabel, currentPropertyRow++, 1 );

    // size property
    label = new QLabel( i18n("Size:"), this );
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight );

    mSizeLabel = new QLabel( this );
    propertyGrid->addWidget( mSizeLabel, currentPropertyRow++, 1 );

#if 0
    label = new QLabel( i18n("Created/Loaded:"), this ); // TODO: make adjustable depending on document
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;

    label = new QLabel( i18n("Last modified:"), this );
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;

    label = new QLabel( i18n("Last synchronized:"), this );
    propertyGrid->addWidget( label, currentPropertyRow, 0, Qt::AlignRight);
    currentPropertyRow++;
// last accessed from remote

    KDateTime dt;// = item.time(KFileItem::CreationTime);
    if ( !dt.isNull() )
    {
      label = new QLabel(i18n("Created:"), this );
      propertyGrid->addWidget(label, currentPropertyRow, 0, Qt::AlignRight);

      label = new QLabel(KGlobal::locale()->formatDateTime(dt), this );
      propertyGrid->addWidget(label, currentPropertyRow++, 2);
    }
#endif

    baseLayout->addLayout( propertyGrid );
    baseLayout->addStretch( 10 );

    connect( mTool, SIGNAL(documentTitleChanged(QString)),
             SLOT(onDocumentTitleChanged(QString)) );
    connect( mTool, SIGNAL(documentMimeTypeChanged(KMimeType::Ptr)),
             SLOT(onMimeTypeChanged(KMimeType::Ptr)) );
    connect( mTool, SIGNAL(locationChanged(QString)),
             SLOT(onLocationChanged(QString)) );
    connect( mTool, SIGNAL(documentSizeChanged(int)),
             SLOT(onDocumentSizeChanged(int)) );
    onDocumentTitleChanged( mTool->documentTitle() );
    onMimeTypeChanged( mTool->mimeType() );
    onLocationChanged( mTool->location() );
    onDocumentSizeChanged( mTool->documentSize() );
}


void DocumentInfoView::onDocumentTitleChanged( const QString& documentTitle )
{
    mDocumentTitleLabel->setText( documentTitle );
}

void DocumentInfoView::onMimeTypeChanged( KMimeType::Ptr mimeTypePtr )
{
    QString mimeTypeComment;
    QPixmap mimeTypeIcon;

    if( mimeTypePtr.isNull() )
    {
        mimeTypeComment = QLatin1String( "-" );
//         mimeTypeIcon = ?
    }
    else
    {
        mimeTypeComment = mimeTypePtr->comment();
        mimeTypeIcon = KIconLoader::global()->loadIcon( mimeTypePtr->iconName(), KIconLoader::Desktop, KIconLoader::SizeEnormous );
    }

    mIconLabel->setPixmap( mimeTypeIcon );
    mMimeTypeLabel->setText( mimeTypeComment );
}

void DocumentInfoView::onLocationChanged( const QString& location )
{
    const QString entry = location.isEmpty() ?
        i18nc( "There is no storage location assigned to yet.", "[None]" ) :
        location;
    mLocationLabel->setText( entry );
}

void DocumentInfoView::onDocumentSizeChanged( int newSize )
{
    const QString size = ( newSize != -1 ) ?
        QString::fromLatin1( "%1 (%2)" )
        .arg( KIO::convertSize(newSize) )
        .arg( KGlobal::locale()->formatNumber(newSize, 0) ) :
        QString::fromLatin1( "-" );
    mSizeLabel->setText( size );
}

DocumentInfoView::~DocumentInfoView() {}

}

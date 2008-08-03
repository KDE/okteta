/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#include "filtertool.h"

// tool
#include "filterjob.h"
// filter
#include <abstractbytearrayfilter.h>
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kabstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QByteArray>


FilterTool::FilterTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 )
{
    setObjectName( "BinaryFilter" );
}

QString FilterTool::title() const { return i18nc("@title:window", "Binary Filter"); }
QString FilterTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->encodingName() : QString();
}

bool FilterTool::dataSelected() const { return ( (mByteArrayView!=0) && mByteArrayView->hasSelectedData() ); }

AbstractByteArrayFilterParameterSet *FilterTool::parameterSet( int filterId )
{
    AbstractByteArrayFilter *byteArrayFilter = mFilterList.at( filterId );

    return byteArrayFilter ? byteArrayFilter->parameterSet() : 0;
}

void FilterTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    KAbstractView* view = model ? qobject_cast<KAbstractView*>( model ) : 0;
    mByteArrayView = view ? qobject_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? qobject_cast<KByteArrayDocument*>( view->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasByteArray = ( mByteArrayModel != 0 );
    QString newCharCodecName;
    if( hasByteArray )
    {
        newCharCodecName = mByteArrayView->encodingName();
        connect( mByteArrayView, SIGNAL(selectionChanged( bool )), SIGNAL(dataSelectionChanged( bool )) );

        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString & )),
                 SIGNAL(charCodecChanged( const QString & )) );
    }

    const bool dataSelected = hasByteArray && mByteArrayView->hasSelectedData();

    emit charCodecChanged( newCharCodecName );
    emit dataSelectionChanged( dataSelected );
}

void FilterTool::addFilter( AbstractByteArrayFilter *filter )
{
    mFilterList.append( filter );
}


void FilterTool::filter( int filterId ) const
{
    AbstractByteArrayFilter *byteArrayFilter = mFilterList.at( filterId );

    if( byteArrayFilter )
    {
        const KHE::KSection filteredSection = mByteArrayView->selection();

        QByteArray filterResult;
        filterResult.resize( filteredSection.width() );

        QApplication::setOverrideCursor( Qt::WaitCursor );

        FilterJob *filterJob = new FilterJob( byteArrayFilter, filterResult.data(), mByteArrayModel, filteredSection );
        const bool success = filterJob->exec();

        QApplication::restoreOverrideCursor();

        if( success )
        {
            KHECore::ChangesDescribable *changesDescribable =
                qobject_cast<KHECore::ChangesDescribable*>( mByteArrayModel );

            if( changesDescribable )
                changesDescribable->openGroupedChange( byteArrayFilter->name() );
            mByteArrayModel->replace( filteredSection, filterResult );
            if( changesDescribable )
                changesDescribable->closeGroupedChange();
        }
    }
}

FilterTool::~FilterTool() {}

#include "filtertool.moc"

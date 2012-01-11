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

#include "filtertool.h"

// tool
#include "filterjob.h"
// filter
#include <bytearrayfilterfactory.h>
#include <abstractbytearrayfilter.h>
// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
#include <changesdescribable.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>
#include <QtCore/QByteArray>


namespace Kasten2
{

FilterTool::FilterTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 ), mHasWritable( false )
{
    setObjectName( QLatin1String( "BinaryFilter" ) );

    mFilterList = ByteArrayFilterFactory::createFilters();
}

QString FilterTool::title() const { return i18nc("@title:window", "Binary Filter"); }
QList<AbstractByteArrayFilter*> FilterTool::filterList() const { return mFilterList; }
QString FilterTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->charCodingName() : QString();
}

bool FilterTool::hasWriteable() const { return mHasWritable; }

AbstractByteArrayFilterParameterSet *FilterTool::parameterSet( int filterId )
{
    AbstractByteArrayFilter *byteArrayFilter = mFilterList.at( filterId );

    return byteArrayFilter ? byteArrayFilter->parameterSet() : 0;
}

void FilterTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument *document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasByteArray = ( mByteArrayModel && mByteArrayView );
    QString newCharCodecName;
    if( hasByteArray )
    {
        newCharCodecName = mByteArrayView->charCodingName();
        connect( mByteArrayView, SIGNAL(hasSelectedDataChanged(bool)), SLOT(onApplyableChanged()) );
        connect( mByteArrayView, SIGNAL(readOnlyChanged(bool)),        SLOT(onApplyableChanged()) );
        connect( mByteArrayView, SIGNAL(charCodecChanged(QString)),
                 SIGNAL(charCodecChanged(QString)) );
    }

    onApplyableChanged();
    emit charCodecChanged( newCharCodecName );
}


void FilterTool::filter( int filterId ) const
{
    AbstractByteArrayFilter *byteArrayFilter = mFilterList.at( filterId );

    if( byteArrayFilter )
    {
        const Okteta::AddressRange filteredSection = mByteArrayView->selection();

        QByteArray filterResult;
        filterResult.resize( filteredSection.width() );

        QApplication::setOverrideCursor( Qt::WaitCursor );

        FilterJob* filterJob = new FilterJob( byteArrayFilter, reinterpret_cast<Okteta::Byte*>(filterResult.data()), mByteArrayModel, filteredSection );
        const bool success = filterJob->exec();

        QApplication::restoreOverrideCursor();

        if( success )
        {
            Okteta::ChangesDescribable *changesDescribable =
                qobject_cast<Okteta::ChangesDescribable*>( mByteArrayModel );

            if( changesDescribable )
                changesDescribable->openGroupedChange( byteArrayFilter->name() );
            mByteArrayModel->replace( filteredSection, filterResult );
            if( changesDescribable )
                changesDescribable->closeGroupedChange();
        }
    }

    mByteArrayView->setFocus();
}

void FilterTool::onApplyableChanged()
{
    const bool newHasWriteable = ( mByteArrayModel && mByteArrayView
                                   && !mByteArrayView->isReadOnly() && mByteArrayView->hasSelectedData() );
    if( newHasWriteable != mHasWritable )
    {
        mHasWritable = newHasWriteable;
        emit hasWriteableChanged( newHasWriteable );
    }
}

FilterTool::~FilterTool()
{
    qDeleteAll( mFilterList );
}

}

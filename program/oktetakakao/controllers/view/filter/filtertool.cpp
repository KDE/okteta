/***************************************************************************
                          filtertool.cpp  -  description
                             -------------------
    begin                : Fri Jan 11 2008
    copyright            : 2008 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "filtertool.h"

//
#include <abstractbytearrayfilter.h>
#include <kbytearraydocument.h>
// Kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// Qt
#include <QtCore/QByteArray>


FilterTool::FilterTool()
 : mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

QString FilterTool::charCodecName() const
{
    return mByteArrayView ? mByteArrayView->encodingName() : QString();
}

bool FilterTool::hasView() const { return ( mByteArrayView != 0 ); }

AbstractByteArrayFilterParameterSet *FilterTool::parameterSet( int filterId )
{
    AbstractByteArrayFilter *byteArrayFilter = mFilterList.at( filterId );

    return byteArrayFilter ? byteArrayFilter->parameterSet() : 0;
}

void FilterTool::setView( KAbstractView *view )
{
//     mByteArrayView->disconnect( this );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    const bool hasView = ( mByteArrayView != 0 );
    emit viewChanged( hasView );
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
        //TODO: support filter of selection
        const KHE::KSection filteredSection = KHE::KSection::fromWidth( 0, mByteArrayModel->size() );

        QByteArray filterResult;
        filterResult.resize( filteredSection.width() );

        const bool success = byteArrayFilter->filter( filterResult.data(), mByteArrayModel, filteredSection );

        if( success )
            mByteArrayModel->replace( filteredSection, filterResult );
    }
}


FilterTool::~FilterTool() {}

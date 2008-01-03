/***************************************************************************
                          infotool.cpp  -  description
                             -------------------
    begin                : Wed Jan 2 2008
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


#include "infotool.h"

// controller
#include "statistictablemodel.h"
//
#include <kbytearraydocument.h>
// Kakao
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <khechar.h>
#include <kcharcodec.h>
#include <kbytearraymodel.h>


InfoTool::InfoTool()
 : mStatisticTableModel( new StatisticTableModel(mByteCount,this) ),
   mByteArrayView( 0 ), mByteArrayModel( 0 )
{
}

StatisticTableModel *InfoTool::statisticTableModel() const { return mStatisticTableModel; }
bool InfoTool::hasByteArrayView() const { return ( mByteArrayView != 0 ); }
int InfoTool::size() const { return (mByteArrayModel!=0) ? mByteArrayModel->size() : 0; }


void InfoTool::setView( KAbstractView *view )
{
    mByteArrayView->disconnect( mStatisticTableModel );
    mByteArrayModel->disconnect( this );

    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? static_cast<KByteArrayDocument*>( view->document() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView )
    {
        mStatisticTableModel->setCharCodec( mByteArrayView->encodingName() );
        mStatisticTableModel->setValueCoding( mByteArrayView->coding() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString & )),
                 mStatisticTableModel, SLOT(setCharCodec( const QString &)) );
        connect( mByteArrayView,  SIGNAL(valueCodingChanged( int )),
                 mStatisticTableModel, SLOT(setValueCoding( int )) );
        connect( mByteArrayModel,  SIGNAL(modificationChanged( bool )),
                 SIGNAL(statisticDirty( bool )) );
    }
    updateStatistic();
    emit byteArrayViewChanged( mByteArrayView != 0 );
}


void InfoTool::updateStatistic()
{
    // reset
    memset( mByteCount, 0, sizeof(mByteCount) );

    const int size = mByteArrayModel ? mByteArrayModel->size() : 0;

    for( int i=0; i<size; ++i )
        ++mByteCount[(unsigned char)mByteArrayModel->datum(i)];

    mStatisticTableModel->update( size );
    emit statisticDirty( false );
}

InfoTool::~InfoTool() {}

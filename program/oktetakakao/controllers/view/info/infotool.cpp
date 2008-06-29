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

#include "infotool.h"

// controller
#include "statistictablemodel.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
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
    if( mByteArrayView ) mByteArrayView->disconnect( mStatisticTableModel );
    if( mByteArrayModel ) mByteArrayModel->disconnect( this );

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
    emit statisticDirty( true );
//     updateStatistic();
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

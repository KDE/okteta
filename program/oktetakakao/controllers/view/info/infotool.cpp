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
#include "createstatisticjob.h"
// lib
#include <kbytearraydocument.h>
// Kakao gui
#include <kabstractview.h>
// Okteta gui
#include <kbytearrayview.h>
// Okteta core
#include <kbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


InfoTool::InfoTool()
 : mStatisticTableModel( new StatisticTableModel(mByteCount,this) ),
   mByteArrayView( 0 ), mByteArrayModel( 0 ), mSourceByteArrayModelUptodate( false ), mSourceByteArrayModel( 0 )
{
    setObjectName( "Info" );
    updateStatistic();
}

QString InfoTool::title() const { return i18nc("@title:window", "Info"); }
StatisticTableModel *InfoTool::statisticTableModel() const { return mStatisticTableModel; }
int InfoTool::size() const { return (mByteArrayModel!=0) ? mByteArrayModel->size() : 0; }
bool InfoTool::isApplyable() const
{
    return ( mByteArrayModel != 0 && mByteArrayView->hasSelectedData() && !isStatisticUptodate() );
}
bool InfoTool::isStatisticUptodate() const
{
    return ( mSourceByteArrayModelUptodate
             && mSourceByteArrayModel == mByteArrayModel
             && mByteArrayView && mSourceSelection == mByteArrayView->selection() );
}


void InfoTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( mStatisticTableModel );
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    KAbstractView* view = model ? qobject_cast<KAbstractView*>( model ) : 0;
    mByteArrayView = view ? static_cast<KHEUI::KByteArrayView *>( view->widget() ) : 0;

    KByteArrayDocument *document = view ? qobject_cast<KByteArrayDocument*>( view->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView )
    {
        mStatisticTableModel->setCharCodec( mByteArrayView->encodingName() );
        mStatisticTableModel->setValueCoding( mByteArrayView->coding() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged( const QString & )),
                 mStatisticTableModel, SLOT(setCharCodec( const QString &)) );
        connect( mByteArrayView,  SIGNAL(valueCodingChanged( int )),
                 mStatisticTableModel, SLOT(setValueCoding( int )) );
        connect( mByteArrayView,  SIGNAL(selectionChanged( bool )),
                 SLOT(onSelectionChanged( bool )) );
    }

    emit statisticDirty( !isStatisticUptodate() );
    emit isApplyableChanged( isApplyable() );
}

void InfoTool::onSelectionChanged( bool hasSelection )
{
// TODO: could be quicker
Q_UNUSED( hasSelection )
    emit statisticDirty( !isStatisticUptodate() );
    emit isApplyableChanged( isApplyable() );
}

void InfoTool::onSourceChanged()
{
    mSourceByteArrayModelUptodate = false;
    emit statisticDirty( true );
    emit isApplyableChanged( isApplyable() );
}


void InfoTool::updateStatistic()
{
    // forget old string source
    if( mSourceByteArrayModel ) mSourceByteArrayModel->disconnect( this );

    QApplication::setOverrideCursor( Qt::WaitCursor );

    const KHE::KSection selection = ( mByteArrayView ? mByteArrayView->selection() : KHE::KSection() );
    CreateStatisticJob *createStatisticJob =
        new CreateStatisticJob( mByteArrayModel, selection, mByteCount );
    const int selectionSize = createStatisticJob->exec();

    QApplication::restoreOverrideCursor();

    mStatisticTableModel->update( selectionSize );

    // remember new string source
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = selection;
    if( mSourceByteArrayModel )
        connect( mSourceByteArrayModel,  SIGNAL(contentsChanged( const KHE::ArrayChangeMetricsList & )),
                 SLOT(onSourceChanged()) );

    mSourceByteArrayModelUptodate = true;
    emit statisticDirty( false );
    emit isApplyableChanged( false );
}

InfoTool::~InfoTool() {}

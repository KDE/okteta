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

#include "infotool.h"

// controller
#include "statistictablemodel.h"
#include "createstatisticjob.h"
// lib
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>


namespace Kasten2
{

InfoTool::InfoTool()
 : mStatisticTableModel( new StatisticTableModel(mByteCount,this) ),
   mByteArrayView( 0 ), mByteArrayModel( 0 ), mSourceByteArrayModelUptodate( false ), mSourceByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "Info" ) );
    updateStatistic();
}

QString InfoTool::title() const { return i18nc("@title:window", "Statistics"); }
StatisticTableModel *InfoTool::statisticTableModel() const { return mStatisticTableModel; }
int InfoTool::size() const { return (mByteArrayModel!=0) ? mByteArrayModel->size() : -1; }
bool InfoTool::isApplyable() const
{
    return ( mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData() && !isStatisticUptodate() );
}
bool InfoTool::isStatisticUptodate() const
{
    return ( mSourceByteArrayModelUptodate
             && mSourceByteArrayModel == mByteArrayModel
             && mByteArrayView && mSourceSelection == mByteArrayView->selection() );
}


void InfoTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView )
    {
        mByteArrayView->disconnect( mStatisticTableModel );
        mByteArrayView->disconnect( this );
    }

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        mStatisticTableModel->setCharCodec( mByteArrayView->charCodingName() );
        mStatisticTableModel->setValueCoding( mByteArrayView->valueCoding() );
        mStatisticTableModel->setUndefinedChar( mByteArrayView->undefinedChar() );
        connect( mByteArrayView,  SIGNAL(charCodecChanged(QString)),
                 mStatisticTableModel, SLOT(setCharCodec(QString)) );
        connect( mByteArrayView,  SIGNAL(valueCodingChanged(int)),
                 mStatisticTableModel, SLOT(setValueCoding(int)) );
        connect( mByteArrayView,  SIGNAL(undefinedCharChanged(QChar)),
                 mStatisticTableModel, SLOT(setUndefinedChar(QChar)) );

        connect( mByteArrayView,  SIGNAL(selectedDataChanged(const Kasten2::AbstractModelSelection*)),
                 SLOT(onSelectionChanged()) );
    }

    emit statisticDirty( !isStatisticUptodate() );
    emit isApplyableChanged( isApplyable() );
}

void InfoTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    emit statisticDirty( !isStatisticUptodate() );
    emit isApplyableChanged( isApplyable() );
}

void InfoTool::onSourceChanged()
{
    mSourceByteArrayModelUptodate = false;
    emit statisticDirty( true );
    emit isApplyableChanged( isApplyable() );
}

void InfoTool::onSourceDestroyed()
{
    mSourceByteArrayModel = 0;
    onSourceChanged();
}

void InfoTool::updateStatistic()
{
    // forget old string source
    if( mSourceByteArrayModel ) mSourceByteArrayModel->disconnect( this );

    QApplication::setOverrideCursor( Qt::WaitCursor );

    const Okteta::AddressRange selection = ( mByteArrayView ? mByteArrayView->selection() : Okteta::AddressRange() );
    CreateStatisticJob *createStatisticJob =
        new CreateStatisticJob( mByteArrayModel, selection, mByteCount );
    const int selectionSize = createStatisticJob->exec();

    QApplication::restoreOverrideCursor();

    mStatisticTableModel->update( selectionSize );

    // remember new string source
    mSourceByteArrayModel = mByteArrayModel;
    mSourceSelection = selection;
    if( mSourceByteArrayModel )
    {
        connect( mSourceByteArrayModel,  SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                 SLOT(onSourceChanged()) );
        connect( mSourceByteArrayModel,  SIGNAL(destroyed()),
                 SLOT(onSourceDestroyed()) );
    }

    mSourceByteArrayModelUptodate = true;
    emit statisticDirty( false );
    emit isApplyableChanged( false );

    if( mByteArrayView )
        mByteArrayView->setFocus();
}

InfoTool::~InfoTool() {}

}

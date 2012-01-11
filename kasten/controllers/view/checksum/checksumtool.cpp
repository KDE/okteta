/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// QCA
// need to have this first, as QCA needs QT_NO_CAST_FROM_ASCII disabled when included
#include <config-qca2.h> //krazy:excludeall=includes
#ifdef HAVE_QCA2
// disable QT_NO_CAST_FROM_ASCII
#ifdef QT_NO_CAST_FROM_ASCII
#undef QT_NO_CAST_FROM_ASCII
#endif
#include <QtCrypto>
#endif

#include "checksumtool.h"

// lib
#include "checksumcalculatejob.h"
//
#include <bytearraychecksumalgorithmfactory.h>
#include <abstractbytearraychecksumalgorithm.h>
// 
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QApplication>

#include <KDebug>


namespace Kasten2
{

ChecksumTool::ChecksumTool()
  : AbstractTool(),
    mChecksumUptodate( false ),
    mSourceByteArrayModelUptodate( false ),
    mAlgorithmId( 0 ),
    mByteArrayView( 0 ), mByteArrayModel( 0 ),
    mSourceAlgorithmId( -1 ),
    mSourceByteArrayModel( 0 )
{
    setObjectName( QLatin1String( "Checksum" ) );

// TODO: find a better place to do and store the initialization
#ifdef HAVE_QCA2
    mQcaInitializer = new QCA::Initializer( QCA::Practical, 64 );
kDebug()<< QCA::supportedFeatures();//Hash::supportedTypes();
#endif

    mAlgorithmList = ByteArrayChecksumAlgorithmFactory::createAlgorithms();
}

QList<AbstractByteArrayChecksumAlgorithm*> ChecksumTool::algorithmList() const { return mAlgorithmList; }

bool ChecksumTool::isApplyable() const
{
    return ( mByteArrayModel && mByteArrayView && mByteArrayView->hasSelectedData() );
}


QString ChecksumTool::title() const { return i18nc("@title:window of the tool to calculate checksums", "Checksum"); }


AbstractByteArrayChecksumParameterSet* ChecksumTool::parameterSet()
{
    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at( mAlgorithmId );

    return algorithm ? algorithm->parameterSet() : 0;
}

void ChecksumTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayView ) mByteArrayView->disconnect( this );

    mByteArrayView = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayView ? qobject_cast<ByteArrayDocument*>( mByteArrayView->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayView && mByteArrayModel )
    {
        connect( mByteArrayView,  SIGNAL(selectedDataChanged(const Kasten2::AbstractModelSelection*)),
                 SLOT(onSelectionChanged()) );
    }

    // TODO: if there is no view, there is nothing calculate a checksum from
    // or this could be the view where we did the checksum from and it did not change
    checkUptoDate();
    emit uptodateChanged( mChecksumUptodate );
    emit isApplyableChanged( isApplyable() );
}


void ChecksumTool::checkUptoDate()
{
    mChecksumUptodate =
        ( mSourceByteArrayModel == mByteArrayModel
          && mByteArrayView && mSourceSelection == mByteArrayView->selection()
          && mSourceAlgorithmId == mAlgorithmId
          && mSourceByteArrayModelUptodate );
}


void ChecksumTool::calculateChecksum()
{
    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at( mAlgorithmId );

    if( algorithm )
    {
        // forget old string source
        if( mSourceByteArrayModel ) mSourceByteArrayModel->disconnect( this );

        QApplication::setOverrideCursor( Qt::WaitCursor );

        ChecksumCalculateJob* checksumCalculateJob =
            new ChecksumCalculateJob( &mCheckSum, algorithm, mByteArrayModel, mByteArrayView->selection() );
        checksumCalculateJob->exec();

        QApplication::restoreOverrideCursor();

        // remember checksum source
        mSourceAlgorithmId = mAlgorithmId;
        mSourceByteArrayModel = mByteArrayModel;
        mSourceSelection = mByteArrayView->selection();
        connect( mSourceByteArrayModel,  SIGNAL(contentsChanged(Okteta::ArrayChangeMetricsList)),
                 SLOT(onSourceChanged()) );
        connect( mSourceByteArrayModel,  SIGNAL(destroyed()),
                 SLOT(onSourceDestroyed()) );

        mChecksumUptodate = true;
        mSourceByteArrayModelUptodate = true;
        emit checksumChanged( mCheckSum );
        emit uptodateChanged( true );
    }
}

void ChecksumTool::setAlgorithm( int algorithmId )
{
    mAlgorithmId = algorithmId;
    checkUptoDate();
    emit uptodateChanged( mChecksumUptodate );
    emit isApplyableChanged( isApplyable() );
}

// TODO: hack!
// better would be to store the parameter set used for the source and compare if equal
// this hack does the same, except for that the source will never be uptodate
void ChecksumTool::resetSourceTool()
{
    mSourceAlgorithmId = -1;

    checkUptoDate();
    emit uptodateChanged( mChecksumUptodate );
    emit isApplyableChanged( isApplyable() );
}

void ChecksumTool::onSelectionChanged()
{
// TODO: could be quicker using the selection data
    checkUptoDate();
    emit uptodateChanged( mChecksumUptodate );
    emit isApplyableChanged( isApplyable() );
}

void ChecksumTool::onSourceChanged()
{
    mChecksumUptodate = false;
    mSourceByteArrayModelUptodate = false;
    emit uptodateChanged( false );
}

void ChecksumTool::onSourceDestroyed()
{
    mSourceByteArrayModel = 0;
    onSourceChanged();
}

ChecksumTool::~ChecksumTool()
{
    qDeleteAll( mAlgorithmList );
#ifdef HAVE_QCA2
    delete mQcaInitializer;
#endif
}

}

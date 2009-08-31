/*
    This file is part of the Okteta Kasten module, part of the KDE project.

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

#include "checksumtool.h"

// lib
#include "checksumcalculatejob.h"
//
#include <abstractbytearraychecksumalgorithm.h>
#include <algorithm/crc32bytearraychecksumalgorithm.h>
#include <algorithm/adler32bytearraychecksumalgorithm.h>
#include <algorithm/modsum8bytearraychecksumalgorithm.h>
#include <algorithm/modsum16bytearraychecksumalgorithm.h>
#include <algorithm/modsum32bytearraychecksumalgorithm.h>
#include <algorithm/modsum64bytearraychecksumalgorithm.h>
#ifdef HAVE_QCA2
#include <algorithm/qca2bytearraychecksumalgorithm.h>
#endif
// 
#include <bytearrayview.h>
#include <bytearraydocument.h>
// Okteta core
#include <abstractbytearraymodel.h>
// KDE
#include <KLocale>
#ifdef HAVE_QCA2
// QCA
#include <QtCrypto>
#endif
// Qt
#include <QtGui/QApplication>

#include <KDebug>


namespace Kasten
{

#ifdef HAVE_QCA2
static inline void addQca2Algorithm( QList<AbstractByteArrayChecksumAlgorithm*>& algorithmList, const QString& name, const char* type )
{
    if( QCA::isSupported(type) )
        algorithmList << new Qca2ByteArrayChecksumAlgorithm( name, QString::fromLatin1(type) );
}
#endif

ChecksumTool::ChecksumTool()
  : AbstractTool(),
    mChecksumUptodate( false ),
    mSourceByteArrayModelUptodate( false ),
    mAlgorithmId( 0 ),
    mByteArrayDisplay( 0 ), mByteArrayModel( 0 ),
    mSourceAlgorithmId( -1 ),
    mSourceByteArrayModel( 0 )
{
    setObjectName( "Checksum" );

    mAlgorithmList
        << new ModSum8ByteArrayChecksumAlgorithm()
        << new ModSum16ByteArrayChecksumAlgorithm()
        << new ModSum32ByteArrayChecksumAlgorithm()
        << new ModSum64ByteArrayChecksumAlgorithm()
        << new Adler32ByteArrayChecksumAlgorithm()
        << new Crc32ByteArrayChecksumAlgorithm();

#ifdef HAVE_QCA2
    mQcaInitializer = new QCA::Initializer( QCA::Practical, 64 );
kDebug()<< QCA::supportedFeatures();//Hash::supportedTypes();

    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-0"),     "sha0" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-1"),     "sha1" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "MD2"),       "md2" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "MD4"),       "md4" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "MD5"),       "md5" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "RIPEMD160"), "ripemd160" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-224"),   "sha224" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-256"),   "sha256" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-384"),   "sha384" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "SHA-512"),   "sha512" );
    addQca2Algorithm( mAlgorithmList, i18nc( "name of the hash algorithm", "Whirlpool"), "whirlpool" );
#endif
}

QList<AbstractByteArrayChecksumAlgorithm*> ChecksumTool::algorithmList() const { return mAlgorithmList; }

bool ChecksumTool::isApplyable() const
{
    return ( mByteArrayModel && mByteArrayDisplay && mByteArrayDisplay->hasSelectedData() );
}


QString ChecksumTool::title() const { return i18nc("@title:window of the tool to calculate checksums", "Checksum"); }


AbstractByteArrayChecksumParameterSet* ChecksumTool::parameterSet()
{
    AbstractByteArrayChecksumAlgorithm* algorithm = mAlgorithmList.at( mAlgorithmId );

    return algorithm ? algorithm->parameterSet() : 0;
}

void ChecksumTool::setTargetModel( AbstractModel* model )
{
    if( mByteArrayDisplay ) mByteArrayDisplay->disconnect( this );

    mByteArrayDisplay = model ? model->findBaseModel<ByteArrayView*>() : 0;

    ByteArrayDocument* document =
        mByteArrayDisplay ? qobject_cast<ByteArrayDocument*>( mByteArrayDisplay->baseModel() ) : 0;
    mByteArrayModel = document ? document->content() : 0;

    if( mByteArrayDisplay && mByteArrayModel )
    {
        connect( mByteArrayDisplay,  SIGNAL(selectedDataChanged( const Kasten::AbstractModelSelection* )),
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
          && mByteArrayDisplay && mSourceSelection == mByteArrayDisplay->selection()
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
            new ChecksumCalculateJob( &mCheckSum, algorithm, mByteArrayModel, mByteArrayDisplay->selection() );
        checksumCalculateJob->exec();

        QApplication::restoreOverrideCursor();

        // remember checksum source
        mSourceAlgorithmId = mAlgorithmId;
        mSourceByteArrayModel = mByteArrayModel;
        mSourceSelection = mByteArrayDisplay->selection();
        connect( mSourceByteArrayModel,  SIGNAL(contentsChanged( const Okteta::ArrayChangeMetricsList& )),
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

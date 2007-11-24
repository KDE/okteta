/***************************************************************************
                          kdocumentcodecmanager.cpp  -  description
                             -------------------
    begin                : Thu Nov 22 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kdocumentcodecmanager.h"

// lib
#include "kabstractdocumentstreamencoder.h"
// #include "kabstractdocumentdatastreamdecoder.h"
#include "kdocumentmanager.h"


KDocumentCodecManager::KDocumentCodecManager( KDocumentManager *manager )
 : mManager( manager ), mWidget( 0 ) {}

QList<KAbstractDocumentStreamEncoder*> KDocumentCodecManager::encoderList() const { return mEncoderList; }
QList<KAbstractDocumentStreamDecoder*> KDocumentCodecManager::decoderList() const { return mDecoderList; }

void KDocumentCodecManager::setWidget( QWidget *widget )
{
    mWidget = widget;
}

void KDocumentCodecManager::setEncoders( QList<KAbstractDocumentStreamEncoder*> &encoderList )
{
    mEncoderList = encoderList;
}

void KDocumentCodecManager::setDecoders( QList<KAbstractDocumentStreamDecoder*> &decoderList )
{
    mDecoderList = decoderList;
}

void KDocumentCodecManager::encodeToStream( KAbstractDocumentStreamEncoder *encoder,
                                            const KAbstractDocumentSelection *selection )
{
   Q_UNUSED( selection )
   Q_UNUSED( encoder )
//    KAbstractDocument *document = mFactory->create();
//    mManager->addDocument( document );
}


KDocumentCodecManager::~KDocumentCodecManager()
{
    qDeleteAll( mEncoderList );
//     qDeleteAll( mDecoderList );
}

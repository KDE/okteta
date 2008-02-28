/*
    This file is part of the Kakao Framework, part of the KDE project.

    Copyright 2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

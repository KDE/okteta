/***************************************************************************
                          kbytearrayeditprivate.cpp  -  description
                             -------------------
    begin                : Die Jul 9 2003
    copyright            : (C) 2003 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License version 2 as published by the Free Software Foundation.       *
 *                                                                         *
 ***************************************************************************/


// qt specific
#include <QtGui/QLayout>
#include <QtGui/QWidget>
//
#include "kbytearrayedit.h"
#include "kbytearrayeditprivate.h"


KByteArrayEditPrivate::KByteArrayEditPrivate( KByteArrayEdit *Parent )
{
    Model = new KHECore::KByteArrayModel();
    HexEdit = new KHEUI::KHexEdit( Model, Parent );

    QHBoxLayout* Layout = new QHBoxLayout( Parent );
    Layout->addWidget( HexEdit );

    Parent->connect( HexEdit, SIGNAL(copyAvailable( bool )), SIGNAL(copyAvailable( bool )) );
}


void KByteArrayEditPrivate::setReadOnly( bool RO )
{
    Model->setReadOnly( RO );
    HexEdit->setReadOnly( RO );
}


void KByteArrayEditPrivate::setAutoDelete( bool AD )
{
    Model->setAutoDelete( AD );
}


char *KByteArrayEditPrivate::data() const
{
    return Model->data();
}


void KByteArrayEditPrivate::setData( char *D, int S, int RS, bool KM )
{
    KHECore::KByteArrayModel *OldModel = Model;
    Model = new KHECore::KByteArrayModel( D, S, RS, KM );

    // get attributes
    Model->setReadOnly( OldModel->isReadOnly() );
    Model->setAutoDelete( OldModel->autoDelete() );

    HexEdit->setByteArrayModel( Model );

    delete OldModel;
}


int KByteArrayEditPrivate::dataSize() const
{
    return Model->size();
}


int KByteArrayEditPrivate::maxDataSize() const
{
    return Model->maxSize();
}


void KByteArrayEditPrivate::setMaxDataSize( int MS )
{
    Model->setMaxSize( MS );
}


bool KByteArrayEditPrivate::keepsMemory() const
{
    return Model->keepsMemory();
}


void KByteArrayEditPrivate::setKeepsMemory( bool KM )
{
    Model->setKeepsMemory( KM );
}


bool KByteArrayEditPrivate::isAutoDelete() const
{
    return Model->autoDelete();
}

void KByteArrayEditPrivate::repaintRange( int i1, int i2 )
{
    Model->signalContentsChanged( i1, i2 );
}

KByteArrayEditPrivate::~KByteArrayEditPrivate()
{
    delete Model;
}

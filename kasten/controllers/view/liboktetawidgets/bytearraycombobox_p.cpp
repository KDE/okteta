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

#include "bytearraycombobox_p.h"
#include "bytearraycombobox.h"

// KDE
#include <KLineEdit>
// Qt
#include <QtGui/QLayout>
#include <QtGui/QAbstractItemView>


namespace Okteta
{

void ByteArrayComboBoxPrivate::init()
{
    Q_Q( ByteArrayComboBox );

    QHBoxLayout *baseLayout = new QHBoxLayout( q );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( 0 );

    mFormatComboBox = new KComboBox( q );
    mFormatComboBox->addItems( ByteArrayValidator::codecNames() );
    q->connect( mFormatComboBox, SIGNAL(activated(int)), SLOT(onFormatChanged(int)) );

    mDataEdit = new KLineEdit( q );
    q->setFocusProxy( mDataEdit );
    q->connect( mDataEdit, SIGNAL(textChanged(const QString&)), SLOT(onDataChanged(const QString&)) );
    QAbstractItemView* formatComboBoxListView = mFormatComboBox->view();
    QObject::connect( formatComboBoxListView, SIGNAL(activated( const QModelIndex& )),
             mDataEdit, SLOT(setFocus()) );
    // TODO: is a workaround for Qt 4.5.1 which doesn't emit activated() for mouse clicks
    QObject::connect( formatComboBoxListView, SIGNAL(pressed( const QModelIndex& )),
             mDataEdit, SLOT(setFocus()) );
    mValidator = new ByteArrayValidator( mDataEdit );
    mDataEdit->setValidator( mValidator );

    baseLayout->addWidget( mFormatComboBox );
    baseLayout->addWidget( mDataEdit );
    q->setTabOrder( mFormatComboBox, mDataEdit );

    onFormatChanged( mFormatComboBox->currentIndex() );
}

void ByteArrayComboBoxPrivate::setCharCodec( const QString& charCodecName )
{
    // update the char string
    const QByteArray currentData = mValidator->toByteArray( mData[ByteArrayValidator::CharCoding] );

    mValidator->setCharCodec( charCodecName );

    const QString dataString = mValidator->toString( currentData );
    mData[ByteArrayValidator::CharCoding] = dataString;

    const bool isCharVisible = ( mFormatComboBox->currentIndex() == ByteArrayValidator::CharCoding );

    if( isCharVisible )
        mDataEdit->setText( dataString );
}


void ByteArrayComboBoxPrivate::onFormatChanged( int index )
{
    Q_Q( ByteArrayComboBox );

    mValidator->setCodec( (ByteArrayValidator::Coding)index );
    mDataEdit->setText( mData[index] );

    emit q->formatChanged( index );
}

void ByteArrayComboBoxPrivate::onDataChanged( const QString& data )
{
    Q_Q( ByteArrayComboBox );

    mData[mFormatComboBox->currentIndex()] = data;

    emit q->dataChanged( mValidator->toByteArray(data) );
}

}

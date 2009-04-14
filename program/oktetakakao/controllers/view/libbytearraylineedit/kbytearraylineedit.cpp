/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kbytearraylineedit.h"

// lib
#include "kbytearrayvalidator.h"
// KDE
#include <KComboBox>
#include <KLineEdit>
// Qt
#include <QtGui/QLayout>


class KByteArrayLineEditPrivate
{
  public:
    KComboBox *mFormatComboBox;
    KLineEdit *mDataEdit;

    QString    mData[KByteArrayValidator::CodecNumber];
    KByteArrayValidator *mValidator;
  public:
    void setup( KByteArrayLineEdit *parent );
    void setCharCodec( const QString &charCodecName );
    QByteArray data() const;
    void onFormatChanged( int index );
    void onDataChanged( const QString &data );
};

void KByteArrayLineEditPrivate::setup( KByteArrayLineEdit *widget )
{
    QVBoxLayout *baseLayout = new QVBoxLayout( widget );
    baseLayout->setMargin( 0 );

    mFormatComboBox = new KComboBox( widget );
    mFormatComboBox->addItems( KByteArrayValidator::codecNames() );
    widget->connect( mFormatComboBox, SIGNAL(activated(int)), SLOT(onFormatChanged(int)) );

    mDataEdit = new KLineEdit( widget );
    widget->setFocusProxy( mDataEdit );
    widget->connect( mDataEdit, SIGNAL(textChanged(const QString&)), SLOT(onDataChanged(const QString&)) );
    mValidator = new KByteArrayValidator( mDataEdit );
    mDataEdit->setValidator( mValidator );

    baseLayout->addWidget( mFormatComboBox );
    baseLayout->addWidget( mDataEdit );
    widget->setTabOrder( mFormatComboBox, mDataEdit );

    widget->onFormatChanged( mFormatComboBox->currentIndex() );
}

void KByteArrayLineEditPrivate::setCharCodec( const QString &charCodecName )
{
    // update the char string
    const QByteArray currentData = mValidator->toByteArray( mData[KByteArrayValidator::CharCoding] );

    mValidator->setCharCodec( charCodecName );

    const QString dataString = mValidator->toString( currentData );
    mData[KByteArrayValidator::CharCoding] = dataString;

    const bool isCharVisible = ( mFormatComboBox->currentIndex() == KByteArrayValidator::CharCoding );

    if( isCharVisible )
        mDataEdit->setText( dataString );
}

inline QByteArray KByteArrayLineEditPrivate::data() const
{
    return mValidator->toByteArray( mData[mFormatComboBox->currentIndex()] );
}
inline void KByteArrayLineEditPrivate::onFormatChanged( int index )
{
    mValidator->setCodec( (KByteArrayValidator::Coding)index );
    mDataEdit->setText( mData[index] );
}
inline void KByteArrayLineEditPrivate::onDataChanged( const QString &data )
{
    mData[mFormatComboBox->currentIndex()] = data;
}


KByteArrayLineEdit::KByteArrayLineEdit( QWidget *parent )
  : QWidget( parent ), d( new KByteArrayLineEditPrivate() )
{
    d->setup( this );
}

void KByteArrayLineEdit::setCharCodec( const QString &charCodecName )
{
    d->setCharCodec( charCodecName );
}

QByteArray KByteArrayLineEdit::data() const
{
    return d->data();
}

int KByteArrayLineEdit::format() const
{
    return d->mFormatComboBox->currentIndex();
}


void KByteArrayLineEdit::onFormatChanged( int index )
{
    d->onFormatChanged( index );
    emit formatChanged( index );
}

void KByteArrayLineEdit::onDataChanged( const QString &data )
{
    d->onDataChanged( data );
    emit dataChanged( d->mValidator->toByteArray(data) );
}

KByteArrayLineEdit::~KByteArrayLineEdit() { delete d; }

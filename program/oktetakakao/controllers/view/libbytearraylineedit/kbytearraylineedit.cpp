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
#include <KDialog>
// Qt
#include <QtGui/QLayout>


class KByteArrayLineEditPrivate
{
  public:
    KComboBox *FormatComboBox;
    KLineEdit *DataEdit;

    QString    Data[5];
    KByteArrayValidator *Validator;
  public:
    void setup( KByteArrayLineEdit *parent );
    QByteArray data() const;
    void onFormatChanged( int Index );
    void onDataChanged( const QString &Data );
};

void KByteArrayLineEditPrivate::setup( KByteArrayLineEdit *Widget )
{
    QVBoxLayout *BaseLayout = new QVBoxLayout( Widget );
    BaseLayout->setSpacing( KDialog::spacingHint() );
    BaseLayout->setMargin( 0 );

    FormatComboBox = new KComboBox( Widget );
    FormatComboBox->addItems( KByteArrayValidator::codingNames() );
    Widget->connect( FormatComboBox, SIGNAL(activated(int)), SLOT(onFormatChanged(int)) );

    DataEdit = new KLineEdit( Widget );
    Widget->connect( DataEdit, SIGNAL(textChanged(const QString&)), SLOT(onDataChanged(const QString&)) );
    Validator = new KByteArrayValidator( DataEdit, KHECore::CharCoding );
    DataEdit->setValidator( Validator );

    BaseLayout->addWidget( FormatComboBox );
    BaseLayout->addWidget( DataEdit );
    Widget->setTabOrder( FormatComboBox, DataEdit );

    Widget->onFormatChanged( FormatComboBox->currentIndex() );
}

inline QByteArray KByteArrayLineEditPrivate::data() const
{
    return Validator->toByteArray( Data[FormatComboBox->currentIndex()] );
}
inline void KByteArrayLineEditPrivate::onFormatChanged( int Index )
{
    Validator->setCoding( Index );
    DataEdit->setText( Data[Index] );
}
inline void KByteArrayLineEditPrivate::onDataChanged( const QString &D )
{
    Data[FormatComboBox->currentIndex()] = D;
}


KByteArrayLineEdit::KByteArrayLineEdit( QWidget *parent )
  : QWidget( parent ), d( new KByteArrayLineEditPrivate() )
{
    d->setup( this );
}

void KByteArrayLineEdit::setCharCode( const QString &CodeName )
{
    d->Validator->setCharCode( CodeName );
}


QByteArray KByteArrayLineEdit::data() const
{
    return d->data();
}

int KByteArrayLineEdit::format() const
{
    return d->FormatComboBox->currentIndex();
}


void KByteArrayLineEdit::onFormatChanged( int Index )
{
    d->onFormatChanged( Index );
    emit formatChanged( Index );
}

void KByteArrayLineEdit::onDataChanged( const QString &Data )
{
    d->onDataChanged( Data );
    emit dataChanged( d->Validator->toByteArray(Data) );
}

KByteArrayLineEdit::~KByteArrayLineEdit() { delete d; }

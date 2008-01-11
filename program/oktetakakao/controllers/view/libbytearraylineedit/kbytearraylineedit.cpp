/***************************************************************************
                          kbytearraylineedit.cpp  -  description
                             -------------------
    begin                : Sun Nov 26 2006
    copyright            : 2006 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#include "kbytearraylineedit.h"

// 
#include <kbytearrayvalidator.h>
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

#include "kbytearraylineedit.moc"

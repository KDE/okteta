/***************************************************************************
                          insertpatterndialog.cpp  -  description
                             -------------------
    begin                : Thu Nov 23 2006
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


#include "insertpatterndialog.h"

// 
#include <kbytearraylineedit.h>
// KDE
#include <KLocale>
// Qt
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>



InsertPatternDialog::InsertPatternDialog( QWidget *parent )
  : KDialog( parent )
{
    setCaption( i18n("Insert Pattern") );
    setButtons( Ok | Cancel );
    setButtonGuiItem( Ok, KGuiItem(i18n("&Insert"), 0,//"insert",
                      i18n("Insert the pattern"),
                      i18n("<qt>If you press the <b>Insert</b> button, the pattern you entered "
                           "above are inserted in the document at the cursor position.</qt>")));
    setDefaultButton( Ok );
    setModal( false );

    QWidget *page = new QWidget( this );
    setMainWidget( page );

    QVBoxLayout *baseLayout = new QVBoxLayout( page );
    baseLayout->setSpacing( spacingHint() );
    baseLayout->setMargin( 0 );

    // search term
    QGroupBox *insertBox = new QGroupBox( i18n("Insert"), page );
    baseLayout->addWidget( insertBox );

    QVBoxLayout *patternLayout = new QVBoxLayout;
    patternLayout->setSpacing( spacingHint() );

    QLabel *label = new QLabel( i18n("Pattern:"), insertBox );
    mPatternEdit = new KByteArrayLineEdit( insertBox );
    connect( mPatternEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onInputChanged(const QByteArray&)) );
    label->setBuddy( mPatternEdit );
    const QString inputWhatsThis =
        i18n("Enter a pattern to search for, or select a previous pattern from the list.");
    label->setWhatsThis( inputWhatsThis );
    mPatternEdit->setWhatsThis( inputWhatsThis );
    patternLayout->addWidget( label );
    patternLayout->addWidget( mPatternEdit );
    insertBox->setLayout( patternLayout );

    label = new QLabel( i18n("&Number:"), insertBox );
    mNumberSpinBox = new QSpinBox( insertBox );
    mNumberSpinBox->setRange( 1, INT_MAX );
    mNumberSpinBox->setValue( 1 );
    label->setBuddy( mNumberSpinBox );
    const QString numberWhatsThis =
        i18n("Enter the number for how often the pattern should be inserted.");
    label->setWhatsThis( numberWhatsThis );
    mNumberSpinBox->setWhatsThis( numberWhatsThis );

    patternLayout->addWidget( label );
    patternLayout->addWidget( mNumberSpinBox );

    // options
//     QGroupBox *OptionsBox = new QGroupBox( i18n("Options"), page );
//     baseLayout->addWidget( OptionsBox );

//     QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );
//     OptionsBoxLayout->setSpacing( spacingHint() );

    setTabOrder( mPatternEdit, mNumberSpinBox );

    enableButtonOk( false );
}

void InsertPatternDialog::setCharCode( const QString &codeName )
{
    mPatternEdit->setCharCode( codeName );
}

int InsertPatternDialog::number() const { return mNumberSpinBox->value(); }

QByteArray InsertPatternDialog::pattern() const
{
    return mPatternEdit->data();
}


void InsertPatternDialog::onInputChanged( const QByteArray &data )
{
    enableButtonOk( !data.isEmpty() );
}


void InsertPatternDialog::showEvent( QShowEvent *event )
{
    KDialog::showEvent( event );
    mPatternEdit->setFocus();
}


InsertPatternDialog::~InsertPatternDialog() {}

/***************************************************************************
                          kinsertpatterndialog.cpp  -  description
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


#include "kinsertpatterndialog.h"

// 
#include <kbytearraylineedit.h>
// KDE
#include <KLocale>
// #include <kstdguiitem.h>
// Qt
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>



KInsertPatternDialog::KInsertPatternDialog( QWidget *parent )
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

    QWidget *Page = new QWidget( this );
    setMainWidget( Page );

    QVBoxLayout *topLayout = new QVBoxLayout( Page );
    topLayout->setSpacing( spacingHint() );
    topLayout->setMargin( 0 );

    // search term
    QGroupBox *InsertBox = new QGroupBox( i18n("Insert"), Page );
    topLayout->addWidget( InsertBox );

    QVBoxLayout *FindBoxLayout = new QVBoxLayout;
    FindBoxLayout->setSpacing( spacingHint() );

    QLabel *label = new QLabel( i18n("Pattern:"), InsertBox );
    PatternEdit = new KByteArrayLineEdit( InsertBox );
    connect( PatternEdit, SIGNAL(dataChanged(const QByteArray&)), SLOT(onInputChanged(const QByteArray&)) );
    label->setBuddy( PatternEdit );
    const QString InputWhatsThis =
        i18n("Enter a pattern to search for, or select a previous pattern from the list.");
    label->setWhatsThis( InputWhatsThis );
    PatternEdit->setWhatsThis( InputWhatsThis );
    FindBoxLayout->addWidget( label );
    FindBoxLayout->addWidget( PatternEdit );
    InsertBox->setLayout( FindBoxLayout );

    label = new QLabel( i18n("&Number:"), InsertBox );
    NumberSpinBox = new QSpinBox( InsertBox );
    NumberSpinBox->setRange( 1, INT_MAX );
    NumberSpinBox->setValue( 1 );
    label->setBuddy( NumberSpinBox );
    const QString NumberWhatsThis =
        i18n("Enter the number for how often the pattern should be inserted.");
    label->setWhatsThis( NumberWhatsThis );
    NumberSpinBox->setWhatsThis( NumberWhatsThis );

    FindBoxLayout->addWidget( label );
    FindBoxLayout->addWidget( NumberSpinBox );

    // options
//     QGroupBox *OptionsBox = new QGroupBox( i18n("Options"), Page );
//     topLayout->addWidget( OptionsBox );

//     QGridLayout *OptionsBoxLayout = new QGridLayout( OptionsBox );
//     OptionsBoxLayout->setSpacing( spacingHint() );

    setTabOrder( PatternEdit, NumberSpinBox );

    enableButtonOk( false );
}

void KInsertPatternDialog::setCharCode( const QString &CodeName )
{
    PatternEdit->setCharCode( CodeName );
}

int KInsertPatternDialog::number() const { return NumberSpinBox->value(); }

QByteArray KInsertPatternDialog::pattern() const
{
    return PatternEdit->data();
}


void KInsertPatternDialog::onInputChanged( const QByteArray &Data )
{
    enableButtonOk( !Data.isEmpty() );
}


void KInsertPatternDialog::showEvent( QShowEvent *e )
{
  KDialog::showEvent(e);
  PatternEdit->setFocus();
}


KInsertPatternDialog::~KInsertPatternDialog() {}

#include "kinsertpatterndialog.moc"

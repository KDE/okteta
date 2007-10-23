/***************************************************************************
                          kreplacedialog.cpp  -  description
                             -------------------
    begin                : Fri Nov 24 2006
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


#include "kreplacedialog.h"

// 
#include <kbytearraylineedit.h>
// KDE
#include <KGlobal>
#include <KLocale>
// #include <kstdguiitem.h>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>



KReplaceDialog::KReplaceDialog( QWidget *parent )
 : KAbstractFindDialog( parent )
{
    setCaption( i18n("Replace Bytes") );
    setButtonGuiItem( Ok, KGuiItem( i18n("&Replace"), "replace",
                      i18n("Start replace"),
                      i18n("<qt>If you press the <b>Replace</b> button, the bytes you entered "
                           "above are searched for within the document and any occurrence is "
                           "replaced with the replacement bytes.</qt>")));
    setModal( true );

    // replace term
    QGroupBox *ReplaceBox = new QGroupBox( i18n("Replace by"), mainWidget() );

    QVBoxLayout *ReplaceBoxLayout = new QVBoxLayout;
    ReplaceBoxLayout->setSpacing( spacingHint() );

    QLabel *EditLabel = new QLabel( i18n("Replacing bytes:"), ReplaceBox );
    ReplaceDataEdit = new KByteArrayLineEdit( ReplaceBox );
    EditLabel->setBuddy( ReplaceDataEdit );
    const QString ReplaceDataEditWhatsThis =
        i18n("Enter a pattern to replace with, or select a previous pattern from the list.");
    EditLabel->setWhatsThis( ReplaceDataEditWhatsThis );
    ReplaceDataEdit->setWhatsThis( ReplaceDataEditWhatsThis );

    ReplaceBoxLayout->addWidget( EditLabel );
    ReplaceBoxLayout->addWidget( ReplaceDataEdit );

    ReplaceBox->setLayout( ReplaceBoxLayout );

//     setTabOrder( LastTabWidget, ReplaceDataFormatComboBox );
//     setTabOrder( ReplaceDataFormatComboBox, ReplaceDataEdit );

    setOperationBox( ReplaceBox );

    //
    PromptCheckBox = new QCheckBox( i18n("&Prompt on replace") );
    PromptCheckBox->setWhatsThis( i18n("Ask before replacing each match found.") );
    setExtraOption( PromptCheckBox );
}


QByteArray KReplaceDialog::replaceData()  const
{
    return ReplaceDataEdit->data();
}

bool KReplaceDialog::prompt() const
{
    return PromptCheckBox->isChecked();
}

void KReplaceDialog::setCharCode( const QString &CodeName )
{
    ReplaceDataEdit->setCharCode( CodeName );
    KAbstractFindDialog::setCharCode( CodeName );
}


KReplaceDialog::~KReplaceDialog() {}

#include "kreplacedialog.moc"

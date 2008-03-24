/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2006-2007 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kreplacedialog.h"

//  lib
#include <kbytearraylineedit.h>
// KDE
#include <KGlobal>
#include <KLocale>
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
    setCaption( i18nc("@title:window","Replace Bytes") );
    setButtonGuiItem( Ok, KGuiItem( i18nc("@action;button", "&Replace"), "edit-find-replace",
                      i18nc("@info:tooltip","Start replace"),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Replace</interface> button, "
                            "the bytes you entered above are searched for within "
                            "the byte array and any occurrence is replaced with "
                            "the replacement bytes.")) );
    setModal( true );

    // replace term
    QGroupBox *ReplaceBox = new QGroupBox( i18nc("@title:group","Replace by"), mainWidget() );

    QVBoxLayout *ReplaceBoxLayout = new QVBoxLayout;
    ReplaceBoxLayout->setSpacing( spacingHint() );

    QLabel *EditLabel = new QLabel( i18nc("@label:textbox","Replacing bytes:"), ReplaceBox );
    ReplaceDataEdit = new KByteArrayLineEdit( ReplaceBox );
    EditLabel->setBuddy( ReplaceDataEdit );
    const QString ReplaceDataEditWhatsThis =
        i18nc("@info:whatsthis","Enter a pattern to replace with, or select a previous pattern from the list.");
    EditLabel->setWhatsThis( ReplaceDataEditWhatsThis );
    ReplaceDataEdit->setWhatsThis( ReplaceDataEditWhatsThis );

    ReplaceBoxLayout->addWidget( EditLabel );
    ReplaceBoxLayout->addWidget( ReplaceDataEdit );

    ReplaceBox->setLayout( ReplaceBoxLayout );

//     setTabOrder( LastTabWidget, ReplaceDataFormatComboBox );
//     setTabOrder( ReplaceDataFormatComboBox, ReplaceDataEdit );

    setOperationBox( ReplaceBox );

    //
    PromptCheckBox = new QCheckBox( i18nc("@option:check","&Prompt on replace") );
    PromptCheckBox->setWhatsThis( i18nc("@info:whatsthis","Ask before replacing each match found.") );
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

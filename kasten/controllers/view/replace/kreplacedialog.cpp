/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

// controller
#include "replacetool.h"
//  lib
#include <bytearraycombobox.h>
// KDE
#include <KGlobal>
#include <KLocale>
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QLayout>


namespace Kasten2
{

KReplaceDialog::KReplaceDialog( ReplaceTool* tool, QWidget* parent )
  : KAbstractFindDialog( parent ),
    mTool( tool )
{
    setCaption( i18nc("@title:window","Replace Bytes") );
    setButtonGuiItem( Ok, KGuiItem( i18nc("@action;button", "&Replace"),
                      QLatin1String("edit-find-replace"),
                      i18nc("@info:tooltip","Start replace"),
                      i18nc("@info:whatsthis",
                            "If you press the <interface>Replace</interface> button, "
                            "the bytes you entered above are searched for within "
                            "the byte array and any occurrence is replaced with "
                            "the replacement bytes.")) );

    setupFindBox();

    // replace term
    QGroupBox *ReplaceBox = new QGroupBox( i18nc("@title:group","Replace With"), mainWidget() );

    QVBoxLayout *ReplaceBoxLayout = new QVBoxLayout;

    ReplaceDataEdit = new Okteta::ByteArrayComboBox( ReplaceBox );
    const QString toolTip =
        i18nc("@info:tooltip",
              "Enter the bytes to replace with, or select bytes previously replaced with from the list.");
    ReplaceDataEdit->setToolTip( toolTip );

    ReplaceBoxLayout->addWidget( ReplaceDataEdit );

    ReplaceBox->setLayout( ReplaceBoxLayout );
    setupOperationBox( ReplaceBox );

    //
    PromptCheckBox = new QCheckBox( i18nc("@option:check","&Prompt on replace") );
    PromptCheckBox->setWhatsThis( i18nc("@info:whatsthis","Ask before replacing each match found.") );

    setupCheckBoxes( PromptCheckBox );

    enableButtonOk( false );
    setModal( true );
}


QByteArray KReplaceDialog::replaceData()  const
{
    return ReplaceDataEdit->byteArray();
}

bool KReplaceDialog::prompt() const
{
    return PromptCheckBox->isChecked();
}

void KReplaceDialog::setCharCodec( const QString &codecName )
{
    ReplaceDataEdit->setCharCodec( codecName );
    KAbstractFindDialog::setCharCodec( codecName );
}


void KReplaceDialog::slotButtonClicked( int button )
{
    if( button != KDialog::Ok )
        KAbstractFindDialog::slotButtonClicked( button );
    else
    {
        hide();

        rememberCurrentSettings();

        mTool->setSearchData( data() );
        mTool->setReplaceData( replaceData() );
        mTool->setCaseSensitivity( caseSensitivity() );
        mTool->setDoPrompt( prompt() );

        mTool->replace( direction(), fromCursor(), inSelection() );
    }
}

void KReplaceDialog::showEvent( QShowEvent* showEvent )
{
    KAbstractFindDialog::showEvent( showEvent );

    setInSelection( mTool->hasSelectedData() );
    setCharCodec( mTool->charCodingName() );
}

void KReplaceDialog::rememberCurrentSettings()
{
    KAbstractFindDialog::rememberCurrentSettings();

    ReplaceDataEdit->rememberCurrentByteArray();
}

KReplaceDialog::~KReplaceDialog() {}

}

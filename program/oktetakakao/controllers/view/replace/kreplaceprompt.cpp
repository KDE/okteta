/***************************************************************************
                          kreplaceprompt.cpp  -  description
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


#include "kreplaceprompt.h"

// KDE
#include <KLocale>
// #include <kstdguiitem.h>


KReplacePrompt::KReplacePrompt( QWidget *parent )
 : KDialog( parent )
{
    setModal( true );
    setCaption( i18n("Replace") );

    setButtons( User3 | User2 | User1 | Close );

    setButtonGuiItem( User1, KGuiItem(i18n("Replace &All")) );
    setButtonGuiItem( User2, KGuiItem(i18n("&Skip")) );
    setButtonGuiItem( User3, KGuiItem(i18n("Replace")) );

    setDefaultButton( User3 );
    showButtonSeparator( false );

    resize(minimumSize());
}

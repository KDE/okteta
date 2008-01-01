/***************************************************************************
                          ksearchdialog.cpp  -  description
                             -------------------
    begin                : Tue Nov 14 2006
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


#include "ksearchdialog.h"

// KDE
#include <KLocale>
// #include <kstandardguiitem.h>


KSearchDialog::KSearchDialog( QWidget *parent )
 : KAbstractFindDialog( parent )
{
    setCaption( i18n("Find Bytes") );
    setButtonGuiItem( Ok, KGuiItem( i18n("&Find"), "edit-find",
                      i18n("Start searching"),
                      i18n("<qt>If you press the <b>Find</b> button, the bytes you entered "
                           "above are searched for within the document.</qt>")));
    setModal( false );
}

KSearchDialog::~KSearchDialog() {}

#include "ksearchdialog.moc"

/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2006-2007,2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "kreplaceprompt.h"

// KDE
#include <KLocale>
// Qt
#include <QtCore/QEventLoop>


namespace Kasten2
{

KReplacePrompt::KReplacePrompt( QWidget* parent )
 : KDialog( parent )
{
    setModal( true );
    setCaption( i18nc("@title:window prompt for iterative replacement","Replace") );

    setButtons( User1 | User2 | User3 | Close );

    setButtonGuiItem( User1, KGuiItem(i18nc("@action:button","Replace &All")) );
    setButtonGuiItem( User2, KGuiItem(i18nc("@action:button","&Skip")) );
    setButtonGuiItem( User3, KGuiItem(i18nc("@action:button","Replace")) );

    setDefaultButton( User3 );
    showButtonSeparator( false );

    resize(minimumSize());
}

ReplaceBehaviour KReplacePrompt::query()
{
    QEventLoop eventLoop;
    mEventLoop = &eventLoop;
    eventLoop.exec();

    return mResult;
}

void KReplacePrompt::slotButtonClicked( int buttonCode )
{
    const struct { int mButtonCode; ReplaceBehaviour mBehaviour; } table[] =
    {
        {User1, ReplaceAll},
        {User2, SkipCurrent},
        {User3,ReplaceCurrent},
        {Close,CancelReplacing}
    };
    const int tableSize = sizeof(table) / sizeof(table[0]);

    for( int i=0; i<tableSize; ++i )
        if( table[i].mButtonCode == buttonCode )
        {
            mResult = table[i].mBehaviour;
            mEventLoop->quit();
            break;
        }
}

}

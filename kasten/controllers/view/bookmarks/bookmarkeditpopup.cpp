/*
    This file is part of the Kasten Framework, made within the KDE community.

    Copyright 2009 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "bookmarkeditpopup.h"

// Okteta core
// #include <bookmark.h>
// KDE
#include <KIcon>
#include <KLineEdit>


namespace Kasten2
{

BookmarkEditPopup::BookmarkEditPopup( QWidget* parent )
  : AbstractLinePopup( parent )
{
    setIcon( KIcon(QLatin1String("bookmark-new")) );

    mBookmarkNameLineEdit = new KLineEdit( this );
    connect( mBookmarkNameLineEdit, SIGNAL(returnPressed()), SLOT(onReturnPressed()) );

    setWidget( mBookmarkNameLineEdit );
}

QString BookmarkEditPopup::name() const { return mBookmarkNameLineEdit->text(); }

void BookmarkEditPopup::setName( const QString& name )
{
    mBookmarkNameLineEdit->setText( name );
    mBookmarkNameLineEdit->selectAll();
}

void BookmarkEditPopup::onReturnPressed()
{
    setResult( 1 );
    close();
}

BookmarkEditPopup::~BookmarkEditPopup() {}

}

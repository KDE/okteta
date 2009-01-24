/*
    This file is part of the Kakao Framework, part of the KDE project.

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
// Qt
#include <QtGui/QLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QApplication>


BookmarkEditPopup::BookmarkEditPopup( QWidget* parent )
  : QFrame( parent, Qt::Popup ),
    mEventLoop( 0 ),
    mResult( 0 )
{
    // TODO: what kind of border should there be? like a QMenu?
    setFrameStyle( QFrame::StyledPanel );

//     setAttribute( Qt::WA_DeleteOnClose );
    setMouseTracking( true );

    QHBoxLayout* baseLayout = new QHBoxLayout( this );
    baseLayout->setMargin( 0 );
    baseLayout->setSpacing( 0 );

    QLabel* iconLabel = new QLabel( this );
    iconLabel->setPixmap( KIcon("bookmark-new").pixmap(22) ); // TODO: correct call, using KDE size

    mBookmarkNameLineEdit = new QLineEdit( this );
    connect( mBookmarkNameLineEdit, SIGNAL(returnPressed()), SLOT(onReturnPressed()) );

    baseLayout->addWidget( iconLabel );
    baseLayout->addWidget( mBookmarkNameLineEdit, 10 );

    qApp->installEventFilter( this );
}


QString BookmarkEditPopup::name() const { return mBookmarkNameLineEdit->text(); }

void BookmarkEditPopup::setPosition( const QPoint& globalPosition )
{
    move( globalPosition.x(), globalPosition.y()-height() );
}

void BookmarkEditPopup::setName( const QString& name )
{
    mBookmarkNameLineEdit->setText( name );
}

void BookmarkEditPopup::setVisible( bool visible )
{
    QWidget::setVisible( visible );
    if( mEventLoop )
        mEventLoop->exit();
}

int BookmarkEditPopup::exec()
{
    mBookmarkNameLineEdit->selectAll();
    mBookmarkNameLineEdit->setFocus();
    show();

    QEventLoop eventLoop;
    mEventLoop = &eventLoop;
    eventLoop.exec();
    mEventLoop = 0;

    return mResult;
}

bool BookmarkEditPopup::eventFilter( QObject* object, QEvent* event )
{
    switch( event->type() )
    {
    case QEvent::WindowDeactivate:
        close();
        break;
    case QEvent::MouseButtonPress:
    case QEvent::Wheel:
        if( !isOrContainsObject(object) )
            close();
        break;
    default:
        break;
    }

    return false;
}

bool BookmarkEditPopup::isOrContainsObject( QObject* object ) const
{
    bool result = false;

    while( object )
    {
        if( object == this )
        {
            result = true;
            break;
        }
        object = object->parent();
    }

    return result;
}

void BookmarkEditPopup::onReturnPressed()
{
    mResult = 1;
    close();
}

BookmarkEditPopup::~BookmarkEditPopup() {}

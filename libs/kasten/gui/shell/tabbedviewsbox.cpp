/*
    This file is part of the Kasten Framework, part of the KDE project.

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

#include "tabbedviewsbox.h"

// KDE
#include <KTabWidget>
// Qt
#include <QtGui/QVBoxLayout>
#include <QtGui/QShortcut>


namespace Kasten
{

TabbedViewsBox::TabbedViewsBox( QWidget* parent )
  : QWidget( parent ),
    mBottomWidget( 0 )
{
    mTabWidget = new KTabWidget();
    setFocusProxy( mTabWidget );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addWidget( mTabWidget );

    mEscapeShortcut = new QShortcut( Qt::Key_Escape, this );
    mEscapeShortcut->setEnabled( false );
    connect( mEscapeShortcut, SIGNAL(activated()), SLOT(onDone()) ); // TODO: better use onCancelled
}


KTabWidget* TabbedViewsBox::tabWidget() const { return mTabWidget; }
QWidget* TabbedViewsBox::bottomWidget() const { return mBottomWidget; }


void TabbedViewsBox::setBottomWidget( QWidget* bottomWidget )
{
    QVBoxLayout* layout = static_cast<QVBoxLayout*>( this->layout() );

    if( mBottomWidget )
    {
        mBottomWidget->disconnect( this );
        layout->removeWidget( mBottomWidget );
        delete mBottomWidget;
    }

    mBottomWidget = bottomWidget;
    if( bottomWidget )
    {
        setFocusProxy( bottomWidget );
        connect( bottomWidget, SIGNAL(done()), SLOT(onDone()) );
        layout->addWidget( bottomWidget );
        bottomWidget->show();
        bottomWidget->setFocus();
    }
    else
        setFocusProxy( mTabWidget );

    mEscapeShortcut->setEnabled( (bottomWidget != 0) );
}

void TabbedViewsBox::onDone()
{
    setBottomWidget( 0 );
}

TabbedViewsBox::~TabbedViewsBox()
{
    delete mBottomWidget;
}

}

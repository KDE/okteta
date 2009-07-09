/*
    This file is part of the Kasten Framework, part of the KDE project.

    Copyright 2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#include "parallelwidget.h"

// lib
#include "titledwidget.h"
// Qt
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QWidgetItem>

ParallelWidget::ParallelWidget( QWidget* parent )
: QWidget( parent ), mCurrentWidget( 0 )
{
    mLayout = new QHBoxLayout( this );
    setLayout( mLayout );
}

QWidget* ParallelWidget::currentWidget() const { return mCurrentWidget; }

void ParallelWidget::addWidget( QWidget* widget, const QString& title )
{
    TitledWidget* titledWidget = new TitledWidget( widget, title, this );
    connect( titledWidget, SIGNAL(focusReceived()), SLOT(onFocusReceived()) );
    mLayout->addWidget( titledWidget );
    mWidgetMap.insert( widget, titledWidget );

    widget->setFocus();
}

void ParallelWidget::removeWidget( QWidget* widget )
{
    TitledWidget* titledWidget = mWidgetMap[widget];

    QLayout* boxLayout = layout();
    int index = boxLayout->indexOf( titledWidget );
    boxLayout->removeWidget( titledWidget );
    titledWidget->deleteLater();

    const int widgetsCount = boxLayout->count();
    if( index >= widgetsCount )
        index = widgetsCount-1;
    if( widgetsCount > 0 )
    {
        QWidgetItem* widgetItem = static_cast<QWidgetItem*>( boxLayout->itemAt(index) );
        TitledWidget* nextTitledWidget = static_cast<TitledWidget*>( widgetItem->widget() );
        nextTitledWidget->widget()->setFocus();
    }
    else
        emit currentChanged( 0 );

//TODO: find next widget and set focus
//     if( currentIndex == -1 || currentIndex == index )
        // simulate signal reaction
//         onCurrentChanged( currentIndex );
    mWidgetMap.remove( widget );
}

void ParallelWidget::setCurrentWidget( QWidget* widget )
{
    widget->setFocus();
}

void ParallelWidget::setTitle( QWidget* widget, const QString& title )
{
    mWidgetMap[widget]->setTitle( title );
}

void ParallelWidget::onFocusReceived()
{
    TitledWidget* titledWidget = qobject_cast<TitledWidget *>( sender() );
    if( titledWidget )
    {
        mCurrentWidget = titledWidget->widget();
        emit currentChanged( mCurrentWidget );
    }
}

ParallelWidget::~ParallelWidget() {}

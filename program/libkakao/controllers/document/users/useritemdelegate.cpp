/*
    This file is part of the Kakao Framework, part of the KDE project.

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

#include "useritemdelegate.h"

// lib
#include "userlisttablemodel.h" // TODO: this dependency should be reversed, no?
// KDE
#include <KGlobalSettings>
#include <KLocale>
// Qt
#include <QtGui/QPen>
#include <QtGui/QPainter>

#include <KDebug>

UserItemDelegate::UserItemDelegate( QObject* parent)
: QItemDelegate( parent )
{
}

void UserItemDelegate::paint( QPainter* painter,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& index ) const
{
    drawBackground( painter, option, index );

    const QString userName = index.model()->data( index, Qt::DisplayRole ).toString();
    const QString connection = index.model()->data( index, UserListTableModel::ConnectionRole ).toString();
    const QPixmap faceIcon = index.model()->data( index, UserListTableModel::FaceIconRole ).value<QPixmap>();


    const int focusFrameHMargin = 2;//style()->pixelMetric( QStyle::PM_FocusFrameHMargin );
    const int focusFrameVMargin = 2;//style()->pixelMetric( QStyle::PM_FocusFrameVMargin );
#if 0
    // draw selection outline
    if (option.state & QStyle::State_Selected) {
        QPen oldPen = painter->pen();
        painter->setPen(option.palette.color(QPalette::Highlight));
        painter->drawRect(option.rect.adjusted(2, 2, -2, -2));
        painter->setPen(oldPen);
    }
#endif
    // draw faceIcon
    const int maxheight = 48;//Background::SCREENSHOT_HEIGHT;
    const int maxwidth = 48;//int( maxheight * m_ratio );
    if( !faceIcon.isNull() )
    {
        const int x = (maxwidth - faceIcon.width()) / 2 + focusFrameHMargin;
        const int y = (maxheight - faceIcon.height()) / 2 + focusFrameVMargin;
        const QRect imgRect = QRect( option.rect.topLeft(), faceIcon.size() ).translated( x, y );
        painter->drawPixmap( imgRect, faceIcon );
    }
    // draw text
    painter->save();
    QFont font = painter->font();
    font.setWeight( QFont::Bold );
    painter->setFont( font );
    const int x = option.rect.left() + focusFrameHMargin * 5 + maxwidth;

    const QRect textRect( x,
                          option.rect.top() + focusFrameHMargin,
                          option.rect.width() - x - focusFrameHMargin * 2,
                          maxheight );
    QString text = userName;
    QString connectionCaption;
    if( false )//!connection.isEmpty() )
    {
        connectionCaption = i18nc("Caption to wallpaper preview, %1 connection userName",
                              "connected by: %1", connection);
        text += '\n'+connectionCaption;
    }
    const QRect boundingRect = painter->boundingRect( textRect, Qt::AlignVCenter|Qt::TextWordWrap, text );
    painter->drawText( boundingRect, Qt::TextWordWrap, userName );
    if( !connection.isEmpty() )
    {
        const QRect userNameRect = painter->boundingRect( boundingRect, Qt::TextWordWrap, userName );
        const QRect connectionRect( userNameRect.bottomLeft(), textRect.size() );
        painter->setFont( KGlobalSettings::smallestReadableFont() );
        painter->drawText( connectionRect, Qt::TextWordWrap, connectionCaption );
    }

    drawFocus( painter, option, boundingRect );

    painter->restore();
}


QSize UserItemDelegate::sizeHint( const QStyleOptionViewItem& item,
                                        const QModelIndex& index) const
{
Q_UNUSED( item )
Q_UNUSED( index )
    const int focusFrameHMargin = 2;//style()->pixelMetric( QStyle::PM_FocusFrameHMargin );
    const int maxheight = 48;//Background::SCREENSHOT_HEIGHT;

    return QSize( 100, maxheight + focusFrameHMargin * 2 );
}

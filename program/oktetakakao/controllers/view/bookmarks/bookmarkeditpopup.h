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

#ifndef BOOKMARKEDITPOPUP_H
#define BOOKMARKEDITPOPUP_H

// Qt
#include <QtGui/QFrame>

// class Bookmark;
class QLineEdit;
class QEventLoop;


// TODO: find better name then popup for this kind of view
class BookmarkEditPopup : public QFrame
{
  Q_OBJECT

  public:
    explicit BookmarkEditPopup( QWidget* parent = 0 );
    virtual ~BookmarkEditPopup();

  public:
    QString name() const;

  public:
    void setPosition( const QPoint& globalPosition );
    void setName( const QString& name );
    int exec();

  protected: // QObject API
    virtual bool eventFilter( QObject* object, QEvent* event );

  protected: // QWidget API
    virtual void setVisible( bool visible );

  public:
    bool isOrContainsObject( QObject* object ) const;
    void adjustRect();

  protected Q_SLOTS:
    void onReturnPressed();

  private:
    QLineEdit* mBookmarkNameLineEdit;

    QEventLoop* mEventLoop;
    int mResult;
};

#endif

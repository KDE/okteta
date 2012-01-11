/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

#ifndef SEARCHTOOL_H
#define SEARCHTOOL_H

// libfinddialog
#include <kfinddirection.h>
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <address.h>

namespace Okteta {
class AbstractByteArrayModel;
}


namespace Kasten2
{

namespace If {
class SearchUserQueryable;
}
class ByteArrayView;


class SearchTool : public AbstractTool
{
  Q_OBJECT

  public:
    SearchTool();
    virtual ~SearchTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public: // status
    QByteArray searchData() const;
//     Qt::CaseSensitivity caseSensitivity() const;
    bool hasSelectedData() const;
    QString charCodingName() const;

    bool isApplyable() const; // candidate for AbstractTool API

  public: // actions
    void search( KFindDirection direction, bool fromCursor, bool inSelection );

  public:
    void setUserQueryAgent( If::SearchUserQueryable* userQueryAgent );

  public Q_SLOTS: // settings
    void setSearchData( const QByteArray& searchData );
    void setCaseSensitivity( Qt::CaseSensitivity caseSensitivity );

  Q_SIGNALS:
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void charCodecChanged( const QString& codecName );

    void dataNotFound();

  protected:
    void doSearch( KFindDirection direction );

  protected: // settings
    QByteArray mSearchData;
    Qt::CaseSensitivity mCaseSensitivity;

  protected: // status
    bool mPreviousFound :1;
    Okteta::Address mSearchFirstIndex;
    Okteta::Address mSearchLastIndex;

  protected: //
    If::SearchUserQueryable* mUserQueryAgent;

  protected: // target
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
};


inline QByteArray SearchTool::searchData() const { return mSearchData; }
// inline Qt::CaseSensitivity SearchTool::caseSensitivity()       const { return mCaseSensitivity; }

}

#endif

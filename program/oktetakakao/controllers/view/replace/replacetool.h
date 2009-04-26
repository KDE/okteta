/*
    This file is part of the Okteta Kakao module, part of the KDE project.

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

#ifndef REPLACETOOL_H
#define REPLACETOOL_H

// libfinddialog
#include <kfinddirection.h>
// Kakao core
#include <abstracttool.h>

namespace KDE { namespace If {
class ReplaceUserQueryable;
}}
class KByteArrayDisplay;
namespace KHECore {
class AbstractByteArrayModel;
}

// TODO: is queryAgent needed, or should the tool better be a state machine? same with search tool
class ReplaceTool : public AbstractTool
{
  Q_OBJECT

  public:
    ReplaceTool();
    virtual ~ReplaceTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public: // status
    QByteArray searchData() const;
    QByteArray replaceData() const;
//     bool ignoreCase() const;
    bool hasSelectedData() const;
    QString charCodingName() const;

    bool isApplyable() const; // candidate for AbstractTool API

  public: // actions
    void replace( KFindDirection direction, bool fromCursor, bool inSelection );

  public:
    void setUserQueryAgent( KDE::If::ReplaceUserQueryable* userQueryAgent );

  public Q_SLOTS: // settings
    void setSearchData( const QByteArray& searchData );
    void setReplaceData( const QByteArray& replaceData );
    void setIgnoreCase( int ignoreCase );
    void setDoPrompt( int doPrompt );

  Q_SIGNALS:
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void finished( bool previousFound, int noOfReplacements );

  protected:
    void doReplace( KFindDirection direction, int startIndex );

  protected Q_SLOTS:
    void onReadOnlyChanged( bool isReadOnly );

  protected: // settings
    QByteArray mSearchData;
    QByteArray mReplaceData;
    bool mIgnoreCase :1;
    bool mDoPrompt :1;

  protected: // status
    bool mPreviousFound :1;
    bool mDoWrap :1;
    int mReplaceFirstIndex;
    int mReplaceLastIndex;

  protected: //
    KDE::If::ReplaceUserQueryable* mUserQueryAgent;

  protected: // target
    KByteArrayDisplay* mByteArrayDisplay;
    KHECore::AbstractByteArrayModel* mByteArrayModel;
};


inline QByteArray ReplaceTool::searchData() const { return mSearchData; }
inline QByteArray ReplaceTool::replaceData() const { return mReplaceData; }
// inline bool ReplaceTool::ignoreCase()       const { return mIgnoreCase; }

#endif

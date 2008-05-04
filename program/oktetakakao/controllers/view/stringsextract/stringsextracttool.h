/*
    This file is part of the Okteta Kakao module, part of the KDE project.

    Copyright 2007-2008 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef STRINGSEXTRACTTOOL_H
#define STRINGSEXTRACTTOOL_H

// tool
#include "containedstring.h"
// Okteta core
#include <ksection.h>
// Qt
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QString>

class KAbstractView;
namespace KHEUI {
class KByteArrayView;
}
namespace KHECore {
class KAbstractByteArrayModel;
class KCharCodec;
}

/**
*/
class StringsExtractTool : public QObject
{
  Q_OBJECT

  public:
    StringsExtractTool();
    virtual ~StringsExtractTool();

  public:
    void setView( KAbstractView *view );

  public: // status
    const QList<ContainedString> *containedStringList() const;
    int minLength() const;
    bool isApplyable() const; // candidate for AbstractTool API
    bool isUptodate() const;
    bool isSelectable() const;

  public Q_SLOTS: // settings
    void setCharCodec( const QString &codecName );
    void setMinLength( int minLength );
    void selectString( int stringId );

  public Q_SLOTS: // actions
    void extractStrings();

  public Q_SLOTS: // actions
    void onSourceModified();

  Q_SIGNALS:
    void uptodateChanged( bool isUptodate );
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void isSelectableChanged( bool isSelectable );

  protected: // created data
    QList<ContainedString> mContainedStringList;
    bool mUptodate;

  protected: // settings
    KHECore::KCharCodec *mCharCodec;
    int mMinLength;

  protected: // sources
    KHEUI::KByteArrayView *mByteArrayView;
    // current
    KHECore::KAbstractByteArrayModel *mByteArrayModel;

    // selection source
    KHE::KSection mSourceSelection;
    // source of strings
    KHECore::KAbstractByteArrayModel *mSourceByteArrayModel;
};

inline const QList<ContainedString> *StringsExtractTool::containedStringList() const { return &mContainedStringList; }
inline int StringsExtractTool::minLength()     const { return mMinLength; }
inline bool StringsExtractTool::isUptodate()   const { return mUptodate; }

#endif

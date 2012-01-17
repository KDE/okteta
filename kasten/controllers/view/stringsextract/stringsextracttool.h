/*
    This file is part of the Okteta Kasten module, made within the KDE community.

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

// lib
#include "oktetakastencontrollers_export.h"
// tool
#include "containedstring.h"
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <addressrange.h>
// Qt
#include <QtCore/QList>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}


namespace Kasten2
{

class ByteArrayView;


/**
*/
class OKTETAKASTENCONTROLLERS_EXPORT StringsExtractTool : public AbstractTool
{
  Q_OBJECT

  public:
    StringsExtractTool();
    virtual ~StringsExtractTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public: // status
    const QList<ContainedString> *containedStringList() const;
    int minLength() const;
    bool isApplyable() const; // candidate for AbstractTool API
    bool isUptodate() const;
    bool canHighlightString() const;

  public Q_SLOTS: // settings
    void setCharCodec( const QString &codecName );
    void setMinLength( int minLength );
    void markString( int stringId );
    void unmarkString();

  public Q_SLOTS: // actions
    void extractStrings();

  Q_SIGNALS:
    void uptodateChanged( bool isUptodate );
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void canHighlightStringChanged( bool canHighlightString );

  private:
    void checkUptoDate();

  private Q_SLOTS:
    void onSelectionChanged();
    void onSourceChanged();
    void onSourceDestroyed();
    void onSourceViewDestroyed();

  private: // created data
    QList<ContainedString> mContainedStringList;
    bool mExtractedStringsUptodate:1;
    bool mSourceByteArrayModelUptodate:1;

  private: // settings
    Okteta::CharCodec *mCharCodec;
    int mMinLength;

  private: // sources
    ByteArrayView* mByteArrayView;
    // current
    Okteta::AbstractByteArrayModel *mByteArrayModel;

    // marked view
    ByteArrayView* mSourceByteArrayView;
    // selection source
    Okteta::AddressRange mSourceSelection;
    // source of strings
    Okteta::AbstractByteArrayModel *mSourceByteArrayModel;
    // minLength source
    int mSourceMinLength;
};


inline const QList<ContainedString> *StringsExtractTool::containedStringList() const { return &mContainedStringList; }
inline int StringsExtractTool::minLength()     const { return mMinLength; }
inline bool StringsExtractTool::isUptodate()   const { return mExtractedStringsUptodate; }

}

#endif

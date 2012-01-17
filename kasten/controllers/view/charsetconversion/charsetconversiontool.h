/*
    This file is part of the Okteta Kasten module, made within the KDE community.

    Copyright 2011 Friedrich W. H. Kossebau <kossebau@kde.org>

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

#ifndef CHARSETCONVERSIONTOOL_H
#define CHARSETCONVERSIONTOOL_H

// lib
#include "oktetakastencontrollers_export.h"
// Kasten core
#include <abstracttool.h>
// Okteta core
#include <byte.h>
// Qt
#include <QtCore/QList>
#include <kastencore.h>

namespace Okteta {
class AbstractByteArrayModel;
class CharCodec;
}
template <class Key, class T> class QMap;


namespace Kasten2
{

class ByteArrayView;


/**
*/
class OKTETAKASTENCONTROLLERS_EXPORT CharsetConversionTool : public AbstractTool
{
  Q_OBJECT

  public:
    enum ConversionDirection { ConvertFrom, ConvertTo };

  public:
    CharsetConversionTool();
    virtual ~CharsetConversionTool();

  public: // AbstractTool API
//     virtual AbstractModel* targetModel() const;
    virtual QString title() const;

    virtual void setTargetModel( AbstractModel* model );

  public: // status
    bool isApplyable() const; // candidate for AbstractTool API

    QString otherCharCodecName() const;
    ConversionDirection conversionDirection() const;
    bool isSubstitutingMissingChars() const;
    Okteta::Byte substituteByte() const;

  public Q_SLOTS: // settings
    void setConversionDirection( int conversionDirection );
    void setOtherCharCodecName( const QString& codecName );
    void setSubstitutingMissingChars( bool isSubstitutingMissingChars );
    void setSubstituteByte( int byte );

  public Q_SLOTS: // actions
    void convertChars();

  Q_SIGNALS:
    void isApplyableChanged( bool isApplyable );  // candidate for AbstractTool API
    void conversionDone( bool success, int convertedBytesCount,
                         const QMap<Okteta::Byte, int>& failedPerByteCount );

  private Q_SLOTS:
    void onViewChanged();

  private: // settings
    QString mOtherCharCodecName;
    ConversionDirection mConversionDirection;
    bool mSubstitutingMissingChars;
    Okteta::Byte mSubstituteByte;

  private: // sources
    ByteArrayView* mByteArrayView;
    Okteta::AbstractByteArrayModel* mByteArrayModel;
};

}

#endif

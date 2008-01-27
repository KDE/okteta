/***************************************************************************
                          kbytearrayvaluestreamencoder.h  -  description
                             -------------------
    begin                : Sat Nov 24 2007
    copyright            : 2007 by Friedrich W. H. Kossebau
    email                : kossebau@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License           *
 *   version 2 as published by the Free Software Foundation.               *
 *                                                                         *
 ***************************************************************************/


#ifndef KBYTEARRAYVALUESTREAMENCODER_H
#define KBYTEARRAYVALUESTREAMENCODER_H

// kakao
#include <kabstractbytearraystreamencoder.h>
// Okteta core
#include <khe.h>
// Qt
#include <QtCore/QString>

class ValueStreamEncoderSettings
{
  public:
    ValueStreamEncoderSettings();
  public:
    KHECore::KCoding coding;
    QString separation;
    QChar undefinedChar;
    QChar substituteChar;
};


class KByteArrayValueStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    KByteArrayValueStreamEncoder();
    virtual ~KByteArrayValueStreamEncoder();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device,
                                     const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                     const KHE::KSection &section );

  protected:
    ValueStreamEncoderSettings mSettings;
};

#endif

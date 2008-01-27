/***************************************************************************
                          kbytearraytextstreamencoder.h  -  description
                             -------------------
    begin                : Fri Nov 23 2007
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


#ifndef KBYTEARRAYTEXTSTREAMENCODER_H
#define KBYTEARRAYTEXTSTREAMENCODER_H

// kakao
#include <kabstractbytearraystreamencoder.h>
// Qt
#include <QtCore/QString>

class TextStreamEncoderSettings
{
  public:
    TextStreamEncoderSettings();
  public:
    QString codecName;
    QChar undefinedChar;
    QChar substituteChar;
};

// TODO: this could rather be one of the default cop
class KByteArrayTextStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    KByteArrayTextStreamEncoder();
    virtual ~KByteArrayTextStreamEncoder();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device,
                                     const KHECore::KAbstractByteArrayModel *byteArrayModel,
                                     const KHE::KSection &section );

  protected:
    TextStreamEncoderSettings mSettings;
};

#endif

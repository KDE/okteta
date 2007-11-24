/***************************************************************************
                          kbytearraysourcecodestreamencoder.h  -  description
                             -------------------
    begin                : Thu Nov 22 2007
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


#ifndef KBYTEARRAYSOURCECODESTREAMENCODER_H
#define KBYTEARRAYSOURCECODESTREAMENCODER_H

// lib
#include <kabstractbytearraystreamencoder.h>
// Qt
#include <QtCore/QString>

class SourceCodeStreamEncoderSettings
{
  public:
    enum PrimitiveDataType
    {
        CharType = 0,
        UnsignedCharType,
        ShortType,
        UnsignedShortType,
        IntegerType,
        UnsignedIntegerType,
        FloatType,
        DoubleType
    };

  public:
    SourceCodeStreamEncoderSettings();
  public:
    QString variableName;
    int  dataType;
    int elementsPerLine;
    bool unsignedAsHexadecimal;
};

// TODO: General synchronizer would load matching encoder and decoder
// manually defined by desktopfile
class KByteArraySourceCodeStreamEncoder : public KAbstractByteArrayStreamEncoder
{
    Q_OBJECT

  public:
    KByteArraySourceCodeStreamEncoder();
    virtual ~KByteArraySourceCodeStreamEncoder();

  protected: // KAbstractByteArrayStreamEncoder API
    virtual bool encodeDataToStream( QIODevice *device, const char *data, int size );

  protected:
    void printFormatted( char *buffer, const char *data, unsigned int dataSize ) const;

  protected:
    SourceCodeStreamEncoderSettings mSettings;
};

#endif

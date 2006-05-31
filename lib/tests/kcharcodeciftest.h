/***************************************************************************
                          kcharcodeciftest.h  -  description
                            -------------------
    begin                : Son Mai 7 2006
    copyright            : (C) 2006 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


#ifndef KCHARCODECIFTEST_H
#define KCHARCODECIFTEST_H


// qt specific
#include <QObject>

namespace KHECore {
class KCharCodec;
}


class KCharCodecIfTest : public QObject
{
  Q_OBJECT

  protected:
    KCharCodecIfTest();

  protected: // our API
    virtual KHECore::KCharCodec *createCodec() = 0;
    virtual void deleteCodec( KHECore::KCharCodec *Codec ) = 0;

  private Q_SLOTS: // test functions
    void init();
    void cleanup();

    void testEncodeDecode();

  private:
    /** pointer to the buffer to test */
    KHECore::KCharCodec *CharCodec;
};


inline KCharCodecIfTest::KCharCodecIfTest() : CharCodec( 0 ) {}

#endif

/***************************************************************************
                          testebcdic1047charcodeccharcodecif.h  -  description
                            -------------------
    begin                : Sa Jan 15 2005
    copyright            : (C) 2005 by Friedrich W. H. Kossebau
    email                : Friedrich.W.H@Kossebau.de
***************************************************************************/

/***************************************************************************
*                                                                         *
*   This library is free software; you can redistribute it and/or         *
*   modify it under the terms of the GNU Library General Public           *
*   License version 2 as published by the Free Software Foundation.       *
*                                                                         *
***************************************************************************/


// app specific
#include "testcharcodecif.h"


#ifndef TEST_KEBCDIC1047CHARCODECCHARCODECIF_H
#define TEST_KEBCDIC1047CHARCODECCHARCODECIF_H

/** 
 *
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 * @testfor KDataBuffer
 */
class KEBCDIC1047CharCodecCharCodecIfTest : public KCharCodecIfTest
{
  Q_OBJECT

  public:
    KEBCDIC1047CharCodecCharCodecIfTest();
    virtual ~KEBCDIC1047CharCodecCharCodecIfTest();

    public: //
      virtual void setup();
      virtual void tearDown();
};

#endif

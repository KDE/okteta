/***************************************************************************
                          testtextcharcodeccharcodecif.h  -  description
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


#ifndef TEST_KTEXTCHARCODECCHARCODECIF_H
#define TEST_KTEXTCHARCODECCHARCODECIF_H

/** 
  * 
  * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
  * @testfor KDataBuffer
  */
class KTextCharCodecCharCodecIfTest : public KCharCodecIfTest
{
  Q_OBJECT

  public:
    KTextCharCodecCharCodecIfTest(const QString &Name);
    virtual ~KTextCharCodecCharCodecIfTest();

  public: //
    virtual void setup();
    virtual void tearDown();
};

#endif

/***************************************************************************
                          testcharcodec.h  -  description
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


// kde specific
#include <kde/test/slottest.h>


#ifndef TEST_KCHARCODEC_H
#define TEST_KCHARCODEC_H

class KInstance;

/** 
 *
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 * @testfor KTextCharCodec
 */
class KCharCodecTest : public KDE::Test::SlotTest
{
  Q_OBJECT

    KInstance *Instance;

  public:
    KCharCodecTest();
    virtual ~KCharCodecTest();

  public slots:
    void testCodecNames();
};

#endif

/***************************************************************************
                          testtextcharcodec.h  -  description
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


#ifndef TEST_KTEXTCHARCODEC_H
#define TEST_KTEXTCHARCODEC_H

class KInstance;

/** 
 *
 * @author Friedrich W. H. Kossebau <Friedrich.W.H@Kossebau.de>
 * @testfor KTextCharCodec
 */
class KTextCharCodecTest : public KDE::Test::SlotTest
{
  Q_OBJECT

    KInstance *Instance;

  public:
    KTextCharCodecTest();
    virtual ~KTextCharCodecTest();

  public slots:
    void testCreateLocal();
    void testCreate();
};

#endif
